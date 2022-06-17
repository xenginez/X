#include "FlexLayout.h"

#include "Widget.h"

namespace
{
	struct flex_line
	{
		XE::uint32 child_begin;
		XE::uint32 child_end;
		XE::float32 size;
	};

	struct flex_item
	{
		float width = NAN;
		float height = NAN;
		float left = NAN;
		float right = NAN;
		float top = NAN;
		float bottom = NAN;
		float padding_left = 0;
		float padding_right = 0;
		float padding_top = 0;
		float padding_bottom = 0;
		float margin_left = 0;
		float margin_right = 0;
		float margin_top = 0;
		float margin_bottom = 0;
		float grow = 0;
		float shrink = 1.0f;
		int order = 0;
		float basis = NAN;

		XE::FlexAlign justify_content = XE::FlexAlign::START;
		XE::FlexAlign align_content = XE::FlexAlign::STRETCH;
		XE::FlexAlign align_items = XE::FlexAlign::STRETCH;
		XE::FlexAlign align_self = XE::FlexAlign::AUTO;
		XE::FlexPosition position = XE::FlexPosition::RELATIVE;
		XE::FlexDirection direction = XE::FlexDirection::COLUMN;
		XE::FlexWrap wrap = XE::FlexWrap::NO_WRAP;

		XE::float32 frame[4];
		flex_item * parent = nullptr;
		XE::Array< flex_item * > children;
	};

	struct flex_layout
	{
		bool wrap;
		bool reverse;
		bool reverse2;
		bool vertical;
		XE::float32 size_dim;
		XE::float32 align_dim;
		XE::uint32 frame_pos_i;
		XE::uint32 frame_pos2_i;
		XE::uint32 frame_size_i;
		XE::uint32 frame_size2_i;

		XE::float32 line_dim;
		XE::float32 flex_dim;
		XE::float32 extra_flex_dim;
		XE::float32 flex_grows;
		XE::float32 flex_shrinks;
		XE::float32 pos2;

		bool need_lines;
		XE::float32 lines_sizes;
		XE::Array< flex_line > lines;
	};


#define LAYOUT_RESET() \
{ \
    layout->line_dim = layout->wrap ? 0 : layout->align_dim; \
    layout->flex_dim = layout->size_dim; \
    layout->extra_flex_dim = 0; \
    layout->flex_grows = 0; \
    layout->flex_shrinks = 0; \
}

#define LAYOUT_CHILD_AT(item, i) (item->children[i])
#define LAYOUT_FRAME(child, name) child->frame[layout->frame_##name##_i]
#define CHILD_POS(child) LAYOUT_FRAME(child, pos)
#define CHILD_POS2(child) LAYOUT_FRAME(child, pos2)
#define CHILD_SIZE(child) LAYOUT_FRAME(child, size)
#define CHILD_SIZE2(child) LAYOUT_FRAME(child, size2)
#define CHILD_MARGIN(child, if_vertical, if_horizontal) (layout->vertical ? child->margin_##if_vertical : child->margin_##if_horizontal)

	void layout_item( flex_item * item, float width, float height );

	void layout_init( flex_item * item, float width, float height, flex_layout * layout )
	{
		XE_ASSERT( item->padding_left >= 0 );
		XE_ASSERT( item->padding_right >= 0 );
		XE_ASSERT( item->padding_top >= 0 );
		XE_ASSERT( item->padding_bottom >= 0 );
		width -= item->padding_left + item->padding_right;
		height -= item->padding_top + item->padding_bottom;
		XE_ASSERT( width >= 0 );
		XE_ASSERT( height >= 0 );

		layout->reverse = false;
		layout->vertical = true;
		switch ( item->direction )
		{
		case XE::FlexDirection::ROW_REVERSE:
			layout->reverse = true;
		case XE::FlexDirection::ROW:
			layout->vertical = false;
			layout->size_dim = width;
			layout->align_dim = height;
			layout->frame_pos_i = 0;
			layout->frame_pos2_i = 1;
			layout->frame_size_i = 2;
			layout->frame_size2_i = 3;
			break;

		case XE::FlexDirection::COLUMN_REVERSE:
			layout->reverse = true;
		case XE::FlexDirection::COLUMN:
			layout->size_dim = height;
			layout->align_dim = width;
			layout->frame_pos_i = 1;
			layout->frame_pos2_i = 0;
			layout->frame_size_i = 3;
			layout->frame_size2_i = 2;
			break;

		default:
			XE_ASSERT( false && "incorrect direction" );
		}

		layout->flex_dim = 0;
		layout->flex_grows = 0;
		layout->flex_shrinks = 0;

		layout->reverse2 = false;
		layout->wrap = item->wrap != XE::FlexWrap::NO_WRAP;
		if ( layout->wrap )
		{
			if ( item->wrap == XE::FlexWrap::WRAP_REVERSE )
			{
				layout->reverse2 = true;
				layout->pos2 = layout->align_dim;
			}
		}
		else
		{
			layout->pos2 = layout->vertical
				? item->padding_left : item->padding_top;
		}

		layout->need_lines = layout->wrap && item->align_content != XE::FlexAlign::START;
	}

	bool layout_align( XE::FlexAlign align, float flex_dim, unsigned int children_count, float * pos_p, float * spacing_p, bool stretch_allowed )
	{
		XE_ASSERT( flex_dim > 0 );

		float pos = 0;
		float spacing = 0;
		switch ( align )
		{
		case XE::FlexAlign::START:
			break;
		case XE::FlexAlign::END:
			pos = flex_dim;
			break;
		case XE::FlexAlign::CENTER:
			pos = flex_dim / 2;
			break;
		case XE::FlexAlign::SPACE_BETWEEN:
			if ( children_count > 0 )
			{
				spacing = flex_dim / ( children_count - 1 );
			}
			break;
		case XE::FlexAlign::SPACE_AROUND:
			if ( children_count > 0 )
			{
				spacing = flex_dim / children_count;
				pos = spacing / 2;
			}
			break;
		case XE::FlexAlign::SPACE_EVENLY:
			if ( children_count > 0 )
			{
				spacing = flex_dim / ( children_count + 1 );
				pos = spacing;
			}
			break;
		case XE::FlexAlign::STRETCH:
			if ( stretch_allowed )
			{
				spacing = flex_dim / children_count;
			}
			break;
		default:
			return false;
		}

		*pos_p = pos;
		*spacing_p = spacing;

		return true;
	}

	XE::FlexAlign child_align( flex_item * child, flex_item * parent )
	{
		XE::FlexAlign align = child->align_self;
		if ( align == XE::FlexAlign::AUTO )
		{
			align = parent->align_items;
		}
		return align;
	}

	void layout_items( flex_item * item, unsigned int child_begin, unsigned int child_end, unsigned int children_count, flex_layout * layout )
	{
		XE_ASSERT( children_count <= ( child_end - child_begin ) );
		if ( children_count <= 0 )
		{
			return;
		}

		if ( layout->flex_dim > 0 && layout->extra_flex_dim > 0 )
		{
			layout->flex_dim += layout->extra_flex_dim;
		}

		float pos = 0;
		float spacing = 0;
		if ( layout->flex_grows == 0 && layout->flex_dim > 0 )
		{
			XE_ASSERT( layout_align( item->justify_content, layout->flex_dim, children_count, &pos, &spacing, false ) && "incorrect justify_content" );

			if ( layout->reverse )
			{
				pos = layout->size_dim - pos;
			}
		}

		if ( layout->reverse )
		{
			pos -= layout->vertical ? item->padding_bottom : item->padding_right;
		}
		else
		{
			pos += layout->vertical ? item->padding_top : item->padding_left;
		}
		if ( layout->wrap && layout->reverse2 )
		{
			layout->pos2 -= layout->line_dim;
		}

		for ( unsigned int i = child_begin; i < child_end; i++ )
		{
			flex_item * child = LAYOUT_CHILD_AT( item, i );
			if ( child->position == XE::FlexPosition::ABSOLUTE )
			{
				continue;
			}

			float flex_size = 0;
			if ( layout->flex_dim > 0 )
			{
				if ( child->grow != 0 )
				{
					CHILD_SIZE( child ) = 0;

					flex_size = ( layout->flex_dim / layout->flex_grows ) * child->grow;
				}
			}
			else if ( layout->flex_dim < 0 )
			{
				if ( child->shrink != 0 )
				{
					flex_size = ( layout->flex_dim / layout->flex_shrinks ) * child->shrink;
				}
			}
			CHILD_SIZE( child ) += flex_size;

			float align_size = CHILD_SIZE2( child );
			float align_pos = layout->pos2 + 0;
			switch ( child_align( child, item ) )
			{
			case XE::FlexAlign::END:
				align_pos += layout->line_dim - align_size - CHILD_MARGIN( child, right, bottom );
				break;
			case XE::FlexAlign::CENTER:
				align_pos += ( layout->line_dim / 2 ) - ( align_size / 2 ) + ( CHILD_MARGIN( child, left, top ) - CHILD_MARGIN( child, right, bottom ) );
				break;
			case XE::FlexAlign::STRETCH:
				if ( align_size == 0 )
				{
					CHILD_SIZE2( child ) = layout->line_dim - ( CHILD_MARGIN( child, left, top ) + CHILD_MARGIN( child, right, bottom ) );
				}
			case XE::FlexAlign::START:
				align_pos += CHILD_MARGIN( child, left, top );
				break;
			default:
				XE_ASSERT( false && "incorrect align_self" );
			}

			CHILD_POS2( child ) = align_pos;

			if ( layout->reverse )
			{
				pos -= CHILD_MARGIN( child, bottom, right );
				pos -= CHILD_SIZE( child );
				CHILD_POS( child ) = pos;
				pos -= spacing;
				pos -= CHILD_MARGIN( child, top, left );
			}
			else
			{
				pos += CHILD_MARGIN( child, top, left );
				CHILD_POS( child ) = pos;
				pos += CHILD_SIZE( child );
				pos += spacing;
				pos += CHILD_MARGIN( child, bottom, right );
			}

			layout_item( child, child->frame[2], child->frame[3] );
		}

		if ( layout->wrap && !layout->reverse2 )
		{
			layout->pos2 += layout->line_dim;
		}

		if ( layout->need_lines )
		{
			flex_line line;

			line.child_begin = child_begin;
			line.child_end = child_end;
			line.size = layout->line_dim;
			layout->lines.push_back( line );

			layout->lines_sizes += line.size;
		}
	}

	void layout_item( flex_item * item, float width, float height )
	{
		if ( item->children.size() == 0 )
		{
			return;
		}

		flex_layout layout_s = { 0 }, * layout = &layout_s;
		layout_init( item, width, height, &layout_s );

		LAYOUT_RESET();

		unsigned int last_layout_child = 0;
		unsigned int relative_children_count = 0;
		for ( unsigned int i = 0; i < item->children.size(); i++ )
		{
			flex_item * child = LAYOUT_CHILD_AT( item, i );

			if ( child->position == XE::FlexPosition::ABSOLUTE )
			{
#define ABSOLUTE_SIZE(val, pos1, pos2, dim) (!isnan(val) ? val : (!isnan(pos1) && !isnan(pos2) ? dim - pos2 - pos1 : 0))

#define ABSOLUTE_POS(pos1, pos2, size, dim) (!isnan(pos1) ? pos1 : (!isnan(pos2) ? dim - size - pos2 : 0))

				float child_width = ABSOLUTE_SIZE( child->width, child->left, child->right, width );
				float child_height = ABSOLUTE_SIZE( child->height, child->top, child->bottom, height );
				float child_x = ABSOLUTE_POS( child->left, child->right, child_width, width );
				float child_y = ABSOLUTE_POS( child->top, child->bottom, child_height, height );

				child->frame[0] = child_x;
				child->frame[1] = child_y;
				child->frame[2] = child_width;
				child->frame[3] = child_height;

				layout_item( child, child->frame[2], child->frame[3] );

#undef ABSOLUTE_POS
#undef ABSOLUTE_SIZE
				continue;
			}

			child->frame[0] = 0;
			child->frame[1] = 0;
			child->frame[2] = child->width;
			child->frame[3] = child->height;

			if ( isnan( CHILD_SIZE( child ) ) )
			{
				CHILD_SIZE( child ) = 0;
			}

			if ( isnan( CHILD_SIZE2( child ) ) )
			{
				if ( layout->wrap )
				{
					layout->need_lines = true;
				}
				else
				{
					CHILD_SIZE2( child ) = ( layout->vertical ? width : height )
						- CHILD_MARGIN( child, left, top )
						- CHILD_MARGIN( child, right, bottom );
				}
			}

			if ( !isnan( child->basis ) )
			{
				XE_ASSERT( child->basis >= 0 );
				CHILD_SIZE( child ) = child->basis;
			}

			float child_size = CHILD_SIZE( child );
			if ( layout->wrap )
			{
				if ( layout->flex_dim < child_size )
				{
					layout_items( item, last_layout_child, i, relative_children_count, layout );

					LAYOUT_RESET();

					last_layout_child = i;
					relative_children_count = 0;
				}

				float child_size2 = CHILD_SIZE2( child );
				if ( !isnan( child_size2 ) && child_size2 > layout->line_dim )
				{
					layout->line_dim = child_size2;
				}
			}

			XE_ASSERT( child->grow >= 0 );
			XE_ASSERT( child->shrink >= 0 );

			layout->flex_grows += child->grow;
			layout->flex_shrinks += child->shrink;

			layout->flex_dim -= child_size + ( CHILD_MARGIN( child, top, left ) + CHILD_MARGIN( child, bottom, right ) );

			relative_children_count++;

			if ( child_size > 0 && child->grow > 0 )
			{
				layout->extra_flex_dim += child_size;
			}
		}

		layout_items( item, last_layout_child, item->children.size(), relative_children_count, layout );

		if ( layout->need_lines && layout->lines.size() > 0 )
		{
			float pos = 0;
			float spacing = 0;
			float flex_dim = layout->align_dim - layout->lines_sizes;
			if ( flex_dim > 0 )
			{
				XE_ASSERT( layout_align( item->align_content, flex_dim, layout->lines.size(), &pos, &spacing, true ) && "incorrect align_content" );
			}

			float old_pos = 0;
			if ( layout->reverse2 )
			{
				pos = layout->align_dim - pos;
				old_pos = layout->align_dim;
			}

			for ( unsigned int i = 0; i < layout->lines.size(); i++ )
			{
				flex_line * line = &layout->lines[i];

				if ( layout->reverse2 )
				{
					pos -= line->size;
					pos -= spacing;
					old_pos -= line->size;
				}

				for ( unsigned int j = line->child_begin; j < line->child_end;
					  j++ )
				{
					flex_item * child = LAYOUT_CHILD_AT( item, j );
					if ( child->position == XE::FlexPosition::ABSOLUTE )
					{
						continue;
					}
					if ( isnan( CHILD_SIZE2( child ) ) )
					{
						CHILD_SIZE2( child ) = line->size + ( item->align_content == XE::FlexAlign::STRETCH ? spacing : 0 );
					}
					CHILD_POS2( child ) = pos + ( CHILD_POS2( child ) - old_pos );
				}

				if ( !layout->reverse2 )
				{
					pos += line->size;
					pos += spacing;
					old_pos += line->size;
				}
			}
		}
	}

#undef CHILD_MARGIN
#undef CHILD_POS
#undef CHILD_POS2
#undef CHILD_SIZE
#undef CHILD_SIZE2
#undef LAYOUT_FRAME
#undef LAYOUT_CHILD_AT
#undef LAYOUT_RESET
}

BEG_META( XE::FlexLayout )
type->Property( "FlexWrap", &XE::FlexLayout::_FlexWrap );
type->Property( "AlignItems", &XE::FlexLayout::_AlignItems );
type->Property( "AlignContent", &XE::FlexLayout::_AlignContent );
type->Property( "JustifyContent", &XE::FlexLayout::_JustifyContent );
type->Property( "FlexPosition", &XE::FlexLayout::_FlexPosition );
type->Property( "FlexDirection", &XE::FlexLayout::_FlexDirection );
type->Property( "Padding", &XE::FlexLayout::_Padding );
type->Property( "Margins", &XE::FlexLayout::_Margins );
END_META()

XE::FlexLayout::FlexLayout()
{

}

XE::FlexLayout::~FlexLayout()
{

}

void XE::FlexLayout::Rebuild( XE::int32 width, XE::int32 height, const XE::Array< XE::WidgetPtr > & children ) const
{
	auto create_item = [&]( flex_item * item, const XE::WidgetPtr & child ) -> flex_item *
	{
		item->width = child->GetRect().width;
		item->height = child->GetRect().height;

		if ( auto _item = DP_CAST< XE::FlexLayoutItem>( child->GetLayoutItem() ) )
		{
			item->grow = _item->GetGrow();
			item->basis = _item->GetBasis();
			item->shrink = _item->GetShrink();
			item->align_self = _item->GetAlignSelf();
		}

		return item;
	};

	XE::Array<flex_item> items( children.size() + 1, XE::MemoryResource::GetFrameMemoryResource() );

	flex_item * root = &items[0];
	{
		root->width = width;
		root->height = height;

		root->wrap = this->GetFlexWrap();
		root->position = this->GetFlexPosition();
		root->align_items = this->GetAlignItems();
		root->direction = this->GetFlexDirection();
		root->align_content = this->GetAlignContent();
		root->justify_content = this->GetJustifyContent();
		root->padding_left = this->GetPadding().GetLeft();
		root->padding_right = this->GetPadding().GetRight();
		root->padding_top = this->GetPadding().GetTop();
		root->padding_bottom = this->GetPadding().GetBottom();
		root->margin_left = this->GetMargins().GetLeft();
		root->margin_right = this->GetMargins().GetRight();
		root->margin_top = this->GetMargins().GetTop();
		root->margin_bottom = this->GetMargins().GetBottom();
	}

	for ( size_t i = 0; i < children.size(); i++ )
	{
		auto item = create_item( &items[i + 1], children[i] );

		item->parent = root;

		root->children.push_back( item );
	}

	layout_item( root, root->width, root->height );

	for ( size_t i = 1; i < items.size(); i++ )
	{
		XE::float32 * frame = items[i].frame;

		children[i - 1]->SetFrame(
			{
				static_cast<XE::int32>( frame[0] ),
				static_cast<XE::int32>( frame[1] ),
				static_cast<XE::int32>( frame[2] ),
				static_cast<XE::int32>( frame[3] )
			} );
	}
}

XE::FlexWrap XE::FlexLayout::GetFlexWrap() const
{
	return _FlexWrap;
}

void XE::FlexLayout::SetFlexWrap( XE::FlexWrap val )
{
	_FlexWrap = val;
}

XE::FlexAlign XE::FlexLayout::GetJustifyContent() const
{
	return _JustifyContent;
}

void XE::FlexLayout::SetJustifyContent( XE::FlexAlign val )
{
	_JustifyContent = val;
}

XE::FlexAlign XE::FlexLayout::GetAlignContent() const
{
	return _AlignContent;
}

void XE::FlexLayout::SetAlignContent( XE::FlexAlign val )
{
	_AlignContent = val;
}

XE::FlexAlign XE::FlexLayout::GetAlignItems() const
{
	return _AlignItems;
}

void XE::FlexLayout::SetAlignItems( XE::FlexAlign val )
{
	_AlignItems = val;
}

XE::FlexPosition XE::FlexLayout::GetFlexPosition() const
{
	return _FlexPosition;
}

void XE::FlexLayout::SetFlexPosition( XE::FlexPosition val )
{
	_FlexPosition = val;
}

XE::FlexDirection XE::FlexLayout::GetFlexDirection() const
{
	return _FlexDirection;
}

void XE::FlexLayout::SetFlexDirection( XE::FlexDirection val )
{
	_FlexDirection = val;
}

const XE::Recti & XE::FlexLayout::GetPadding() const
{
	return _Padding;
}

void XE::FlexLayout::SetPadding( const XE::Recti & val )
{
	_Padding = val;
}

const XE::Recti & XE::FlexLayout::GetMargins() const
{
	return _Margins;
}

void XE::FlexLayout::SetMargins( const XE::Recti & val )
{
	_Margins = val;
}

BEG_META( XE::FlexLayoutItem )
type->Property( "Grow", &XE::FlexLayoutItem::_Grow );
type->Property( "Shrink", &XE::FlexLayoutItem::_Shrink );
type->Property( "Basis", &XE::FlexLayoutItem::_Basis );
type->Property( "AlignSelf", &XE::FlexLayoutItem::_AlignSelf );
END_META()

XE::FlexLayoutItem::FlexLayoutItem()
{

}

XE::FlexLayoutItem::~FlexLayoutItem()
{

}

XE::float32 XE::FlexLayoutItem::GetGrow() const
{
	return _Grow;
}

void XE::FlexLayoutItem::SetGrow( XE::float32 val )
{
	_Grow = val;
}

XE::float32 XE::FlexLayoutItem::GetShrink() const
{
	return _Shrink;
}

void XE::FlexLayoutItem::SetShrink( XE::float32 val )
{
	_Shrink = val;
}

XE::float32 XE::FlexLayoutItem::GetBasis() const
{
	return _Basis;
}

void XE::FlexLayoutItem::SetBasis( XE::float32 val )
{
	_Basis = val;
}

XE::FlexAlign XE::FlexLayoutItem::GetAlignSelf() const
{
	return _AlignSelf;
}

void XE::FlexLayoutItem::SetAlignSelf( XE::FlexAlign val )
{
	_AlignSelf = val;
}
