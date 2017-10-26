#include "subwindow_node_list.h"

#include <nanovg.h>
#include <GLFW/glfw3.h>

#include "config.h"

#include "button_category.h"
#include "buttons_nodes.h"
#include "gui_sizes.h"
#include "input_box.h"
#include "node_colors.h"
#include "node_inputs.h"
#include "node_interop_max.h"
#include "node_outputs.h"
#include "node_shaders.h"
#include "node_textures.h"
#include "node_vector.h"

CyclesShaderEditor::NodeListSubwindow::NodeListSubwindow(Point2 screen_position) : NodeEditorSubwindow(screen_position)
{
	subwindow_width = UI_SUBWIN_NODE_LIST_WIDTH;
	subwindow_height = UI_SUBWIN_HEADER_HEIGHT;

	// Category buttons -- cleaned up in ~NodeListSubwindow
	NodeCategoryButton* cat_input_button = new NodeCategoryButton(std::string("Input"));
	NodeCategoryButton* cat_shader_button = new NodeCategoryButton(std::string("Shader"));
	NodeCategoryButton* cat_texture_button = new NodeCategoryButton(std::string("Texture"));
	NodeCategoryButton* cat_color_button = new NodeCategoryButton(std::string("Color"));
	NodeCategoryButton* cat_vector_button = new NodeCategoryButton(std::string("Vector"));

	category_buttons.push_back(cat_input_button);
	category_buttons.push_back(cat_shader_button);
	category_buttons.push_back(cat_texture_button);
	category_buttons.push_back(cat_color_button);
	category_buttons.push_back(cat_vector_button);

	// Input buttons -- cleaned up in ~NodeCategoryButton
	NodeCreationButton* camera_data_button = new GenericNodeButton<CameraDataNode>();
	NodeCreationButton* fresnel_button = new GenericNodeButton<FresnelNode>();
	NodeCreationButton* light_path_button = new GenericNodeButton<LightPathNode>();
	NodeCreationButton* layer_weight_button = new GenericNodeButton<LayerWeightNode>();
	NodeCreationButton* tangent_button = new GenericNodeButton<TangentNode>();
	NodeCreationButton* tex_coord_button = new GenericNodeButton<TextureCoordinateNode>();

	cat_input_button->node_buttons.push_back(camera_data_button);
	cat_input_button->node_buttons.push_back(fresnel_button);
	cat_input_button->node_buttons.push_back(light_path_button);
	cat_input_button->node_buttons.push_back(layer_weight_button);
	cat_input_button->node_buttons.push_back(tangent_button);
	cat_input_button->node_buttons.push_back(tex_coord_button);

	// Shaders buttons -- same ownership as input buttons
	NodeCreationButton* add_button = new GenericNodeButton<AddShaderNode>();
	NodeCreationButton* anisotropic_button = new GenericNodeButton<AnisotropicBSDFNode>();
	NodeCreationButton* diffuse_button = new GenericNodeButton<DiffuseBSDFNode>();
	NodeCreationButton* emission_button = new GenericNodeButton<EmissionNode>();
	NodeCreationButton* glass_button = new GenericNodeButton<GlassBSDFNode>();
	NodeCreationButton* glossy_button = new GenericNodeButton<GlossyBSDFNode>();
	NodeCreationButton* hair_button = new GenericNodeButton<HairBSDFNode>();
	NodeCreationButton* holdout_button = new GenericNodeButton<HoldoutNode>();
	NodeCreationButton* mix_button = new GenericNodeButton<MixShaderNode>();
	NodeCreationButton* principled_button = new GenericNodeButton<PrincipledBSDFNode>();
	NodeCreationButton* refraction_button = new GenericNodeButton<RefractionBSDFNode>();
	NodeCreationButton* sss_button = new GenericNodeButton<SubsurfaceScatteringNode>();
	NodeCreationButton* toon_button = new GenericNodeButton<ToonBSDFNode>();
	NodeCreationButton* translucent_button = new GenericNodeButton<TranslucentBSDFNode>();
	NodeCreationButton* transparent_button = new GenericNodeButton<TransparentBSDFNode>();
	NodeCreationButton* velvet_button = new GenericNodeButton<VelvetBSDFNode>();
	NodeCreationButton* vol_absorption_button = new GenericNodeButton<VolumeAbsorptionNode>();
	NodeCreationButton* vol_scatter_button = new GenericNodeButton<VolumeScatterNode>();

	cat_shader_button->node_buttons.push_back(add_button);
	cat_shader_button->node_buttons.push_back(anisotropic_button);
	cat_shader_button->node_buttons.push_back(diffuse_button);
	cat_shader_button->node_buttons.push_back(emission_button);
	cat_shader_button->node_buttons.push_back(glass_button);
	cat_shader_button->node_buttons.push_back(glossy_button);
	cat_shader_button->node_buttons.push_back(hair_button);
	cat_shader_button->node_buttons.push_back(holdout_button);
	cat_shader_button->node_buttons.push_back(mix_button);
	cat_shader_button->node_buttons.push_back(principled_button);
	cat_shader_button->node_buttons.push_back(refraction_button);
	cat_shader_button->node_buttons.push_back(sss_button);
	cat_shader_button->node_buttons.push_back(toon_button);
	cat_shader_button->node_buttons.push_back(translucent_button);
	cat_shader_button->node_buttons.push_back(transparent_button);
	cat_shader_button->node_buttons.push_back(velvet_button);
	cat_shader_button->node_buttons.push_back(vol_absorption_button);
	cat_shader_button->node_buttons.push_back(vol_scatter_button);

	// Texture buttons -- same ownership as input buttons
#ifdef INCLUDE_MAX_INTEGRATION
	NodeCreationButton* max_tex_button = new GenericNodeButton<MaxTexmapShaderNode>();
#endif
	NodeCreationButton* checker_tex_button = new GenericNodeButton<CheckerTextureNode>();
	NodeCreationButton* gradient_tex_button = new GenericNodeButton<GradientTextureNode>();
	NodeCreationButton* magic_tex_button = new GenericNodeButton<MagicTextureNode>();
	NodeCreationButton* musgrave_tex_button = new GenericNodeButton<MusgraveTextureNode>();
	NodeCreationButton* noise_tex_button = new GenericNodeButton<NoiseTextureNode>();
	NodeCreationButton* voronoi_tex_button = new GenericNodeButton<VoronoiTextureNode>();
	NodeCreationButton* wave_tex_button = new GenericNodeButton<WaveTextureNode>();

	cat_texture_button->node_buttons.push_back(max_tex_button);
	cat_texture_button->node_buttons.push_back(checker_tex_button);
	cat_texture_button->node_buttons.push_back(gradient_tex_button);
	cat_texture_button->node_buttons.push_back(magic_tex_button);
	cat_texture_button->node_buttons.push_back(musgrave_tex_button);
	cat_texture_button->node_buttons.push_back(noise_tex_button);
	cat_texture_button->node_buttons.push_back(voronoi_tex_button);
	cat_texture_button->node_buttons.push_back(wave_tex_button);

	// Color buttons -- same ownership as input buttons
	NodeCreationButton* bright_contrast_button = new GenericNodeButton<BrightnessContrastNode>();
	NodeCreationButton* gamma_button = new GenericNodeButton<GammaNode>();
	NodeCreationButton* hsv_button = new GenericNodeButton<HSVNode>();
	NodeCreationButton* invert_button = new GenericNodeButton<InvertNode>();
	NodeCreationButton* light_falloff_button = new GenericNodeButton<LightFalloffNode>();
	NodeCreationButton* mix_rgb_button = new GenericNodeButton<MixRGBNode>();

	cat_color_button->node_buttons.push_back(bright_contrast_button);
	cat_color_button->node_buttons.push_back(gamma_button);
	cat_color_button->node_buttons.push_back(hsv_button);
	cat_color_button->node_buttons.push_back(invert_button);
	cat_color_button->node_buttons.push_back(light_falloff_button);
	cat_color_button->node_buttons.push_back(mix_rgb_button);

	// Vector buttons -- same ownership as input buttons
	NodeCreationButton* bump_button = new GenericNodeButton<BumpNode>();
	NodeCreationButton* normal_map_button = new GenericNodeButton<NormalMapNode>();

	cat_vector_button->node_buttons.push_back(bump_button);
	cat_vector_button->node_buttons.push_back(normal_map_button);

	active_button = nullptr;

	active_category = cat_shader_button;
}

CyclesShaderEditor::NodeListSubwindow::~NodeListSubwindow()
{
	for (NodeCategoryButton* category_button : category_buttons) {
		delete category_button;
	}
}

void CyclesShaderEditor::NodeListSubwindow::draw(NVGcontext* draw_context)
{
	float height_drawn = 0.0f;

	for (NodeCategoryButton* category_button : category_buttons) {
		if (category_button == active_category) {
			category_button->pressed = true;
		}
		else {
			category_button->pressed = false;
		}
	}

	// Draw window
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, subwindow_width, subwindow_height, UI_SUBWIN_CORNER_RADIUS);
	nvgFillColor(draw_context, nvgRGBA(180, 180, 180, 255));
	nvgFill(draw_context);

	// Header
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, subwindow_width, UI_SUBWIN_HEADER_HEIGHT, UI_SUBWIN_CORNER_RADIUS);
	nvgRect(draw_context, 0.0f, UI_SUBWIN_HEADER_HEIGHT / 2, subwindow_width, UI_SUBWIN_HEADER_HEIGHT / 2);
	if (is_mouse_over_header()) {
		nvgFillColor(draw_context, nvgRGBA(225, 225, 225, 255));
	}
	else {
		nvgFillColor(draw_context, nvgRGBA(210, 210, 210, 255));
	}
	nvgFill(draw_context);

	nvgStrokeColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgStrokeWidth(draw_context, 0.8f);

	nvgBeginPath(draw_context);
	nvgMoveTo(draw_context, 0.0f, UI_SUBWIN_HEADER_HEIGHT);
	nvgLineTo(draw_context, subwindow_width, UI_SUBWIN_HEADER_HEIGHT);
	nvgStroke(draw_context);

	// Outline
	nvgBeginPath(draw_context);
	nvgRoundedRect(draw_context, 0.0f, 0.0f, subwindow_width, subwindow_height, UI_SUBWIN_CORNER_RADIUS);
	nvgStroke(draw_context);

	// Title
	nvgFontSize(draw_context, UI_FONT_SIZE_NORMAL);
	nvgFontFace(draw_context, "sans");
	nvgTextAlign(draw_context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
	nvgFontBlur(draw_context, 0.0f);
	nvgFillColor(draw_context, nvgRGBA(0, 0, 0, 255));
	nvgText(draw_context, subwindow_width / 2, UI_SUBWIN_HEADER_HEIGHT / 2, "Nodes", nullptr);

	height_drawn += (UI_SUBWIN_HEADER_HEIGHT + 3.0f);

	// Node category buttons
	NodeCategoryButtonPlacer placer(Point2(0.0f, height_drawn), subwindow_width, UI_SUBWIN_NODE_LIST_BUTTON_VPADDING);

	for (NodeCategoryButton* category_button : category_buttons) {
		Point2 button_position = placer.next_button_position();
		category_button->update_mouse_position(mouse_local_pos - button_position);
		category_button->draw(button_position, draw_context);
	}

	height_drawn += placer.get_draw_height();

	// Buttons
	if (active_category != nullptr) {
		for (NodeCreationButton* node_button : active_category->node_buttons) {
			Point2 button_location(0.0f, height_drawn);
			height_drawn += node_button->draw_button(draw_context, button_location, mouse_local_pos, subwindow_width);
		}
	}

	subwindow_height = height_drawn + 6.0f;
}

void CyclesShaderEditor::NodeListSubwindow::handle_mouse_button(int button, int action, int /*mods*/) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (is_mouse_over_header()) {
			move_window_begin();
		}
		else if (get_category_button_under_mouse() != nullptr) {
			active_category = get_category_button_under_mouse();
		}
		else if (get_button_under_mouse() != nullptr) {
			if (active_button != nullptr) {
				active_button->pressed = false;
				active_button = nullptr;
			}
			NodeCreationButton* button = get_button_under_mouse();
			button->pressed = true;
			active_button = button;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (subwindow_moving) {
			move_window_end();
		}
		else {
			if (active_button != nullptr) {
				active_button->pressed = false;
				active_button = nullptr;
			}
		}
	}
}

CyclesShaderEditor::NodeCategoryButton* CyclesShaderEditor::NodeListSubwindow::get_category_button_under_mouse()
{
	for (NodeCategoryButton* category_button : category_buttons) {
		if (category_button->is_mouse_over_button()) {
			return category_button;
		}
	}

	return nullptr;
}

CyclesShaderEditor::NodeCreationButton* CyclesShaderEditor::NodeListSubwindow::get_button_under_mouse()
{
	if (active_category == nullptr) {
		return nullptr;
	}

	for (NodeCreationButton* node_button : active_category->node_buttons) {
		if (node_button->is_mouse_over_button()) {
			return node_button;
		}
	}

	return nullptr;
}

bool CyclesShaderEditor::NodeListSubwindow::is_mouse_over_header()
{
	if (subwindow_moving) {
		return true;
	}
	return (mouse_local_pos.get_pos_x() > 0 &&
		mouse_local_pos.get_pos_x() < subwindow_width &&
		mouse_local_pos.get_pos_y() > 0 &&
		mouse_local_pos.get_pos_y() < UI_SUBWIN_HEADER_HEIGHT);
}