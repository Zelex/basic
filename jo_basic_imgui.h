#pragma once

static node_idx_t native_imgui_main_menu_bar(env_ptr_t env, list_ptr_t args) {
    if(ImGui::BeginMainMenuBar()) {
        node_idx_t ret = eval_node_list(env, args);
        ImGui::EndMainMenuBar();
        return ret;
    }
    return NIL_NODE;
}

static node_idx_t native_imgui_menu_bar(env_ptr_t env, list_ptr_t args) {
    if(ImGui::BeginMenuBar()) {
        node_idx_t ret = eval_node_list(env, args);
        ImGui::EndMenuBar();
        return ret;
    }
    return NIL_NODE;
}

static node_idx_t native_imgui_menu(env_ptr_t env, list_ptr_t args) {
    if(ImGui::BeginMenu(get_node_string(args->first_value()).c_str())) {
        node_idx_t ret = eval_node_list(env, args->pop_front());
        ImGui::EndMenu();
        return ret;
    }
    return NIL_NODE;
}

static node_idx_t native_imgui_menu_item(env_ptr_t env, list_ptr_t args) {
    if(ImGui::MenuItem(get_node_string(args->first_value()).c_str())) {
        return eval_node_list(env, args->pop_front());
    }
    return NIL_NODE;
}

static node_idx_t native_imgui_set_next_window_pos(env_ptr_t env, list_ptr_t args) {
    list_t::iterator it(args);
    ImVec2 pos = ImVec2(get_node_float(*it++), get_node_float(*it++));
    int flags = 0;
    for(; it; ++it) {
        jo_string str = get_node_string(*it);
        if(str == "none")            flags |= ImGuiCond_None;
        else if(str == "always")     flags |= ImGuiCond_Always;
        else if(str == "once")       flags |= ImGuiCond_Once;
        else if(str == "first-use-ever")  flags |= ImGuiCond_FirstUseEver;
        else if(str == "appearing")  flags |= ImGuiCond_Appearing;
    }
    ImGui::SetNextWindowPos(pos, flags);
    return NIL_NODE;
}

static node_idx_t native_imgui_set_next_window_size(env_ptr_t env, list_ptr_t args) {
    list_t::iterator it(args);
    ImVec2 pos = ImVec2(get_node_float(*it++), get_node_float(*it++));
    int flags = 0;
    for(; it; ++it) {
        jo_string str = get_node_string(*it);
        if(str == "none")            flags |= ImGuiCond_None;
        else if(str == "always")     flags |= ImGuiCond_Always;
        else if(str == "once")       flags |= ImGuiCond_Once;
        else if(str == "first-use-ever")  flags |= ImGuiCond_FirstUseEver;
        else if(str == "appearing")  flags |= ImGuiCond_Appearing;
    }
    ImGui::SetNextWindowSize(pos, flags);
    return NIL_NODE;
}

static node_idx_t native_imgui_begin(env_ptr_t env, list_ptr_t args) {
    list_t::iterator it(args);
    jo_string name = get_node_string(eval_node(env, *it++));
    node_idx_t p_open = eval_node(env, *it++);
    if(get_node_type(p_open) != NODE_ATOM) {
        warnf("imgui/begin: p_open must be an atom\n");
        return NIL_NODE;
    }
    node_idx_t flags_idx = *it++;
    int flags = 0;
    seq_iterate(flags_idx, [&flags](node_idx_t flag) {
        jo_string str = get_node_string(flag);
        if(str == "none")                   flags |= ImGuiWindowFlags_None;
        else if(str == "no-title-bar")      flags |= ImGuiWindowFlags_NoTitleBar;
        else if(str == "no-resize")         flags |= ImGuiWindowFlags_NoResize;
        else if(str == "no-move")           flags |= ImGuiWindowFlags_NoMove;
        else if(str == "no-scrollbar")      flags |= ImGuiWindowFlags_NoScrollbar;
        else if(str == "no-scroll-with-mouse")  flags |= ImGuiWindowFlags_NoScrollWithMouse;
        else if(str == "no-collapse")       flags |= ImGuiWindowFlags_NoCollapse;
        else if(str == "always-auto-resize")flags |= ImGuiWindowFlags_AlwaysAutoResize;
        else if(str == "no-background")     flags |= ImGuiWindowFlags_NoBackground;
        else if(str == "no-saved-settings") flags |= ImGuiWindowFlags_NoSavedSettings;
        else if(str == "no-mouse-inputs")   flags |= ImGuiWindowFlags_NoMouseInputs;
        else if(str == "menu-bar")          flags |= ImGuiWindowFlags_MenuBar;
        else if(str == "horizontal-scrollbar")  flags |= ImGuiWindowFlags_HorizontalScrollbar;
        else if(str == "no-focus-on-appearing")  flags |= ImGuiWindowFlags_NoFocusOnAppearing;
        else if(str == "no-bring-to-front-on-focus")  flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
        else if(str == "always-vertical-scrollbar")  flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
        else if(str == "always-horizontal-scrollbar")  flags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
        else if(str == "always-use-window-padding")  flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;
        else if(str == "no-nav-inputs")     flags |= ImGuiWindowFlags_NoNavInputs;
        else if(str == "no-nav-focus")      flags |= ImGuiWindowFlags_NoNavFocus;
        else if(str == "unsaved-document")  flags |= ImGuiWindowFlags_UnsavedDocument;
        else if(str == "no-nav")            flags |= ImGuiWindowFlags_NoNav;
        else if(str == "no-decorations")    flags |= ImGuiWindowFlags_NoDecoration;
        else if(str == "no-inputs")         flags |= ImGuiWindowFlags_NoInputs;
        return true;
    });
    
    bool was_open = get_node_bool(node_deref(env, p_open));
    bool open = was_open;
    if(ImGui::Begin(name.c_str(), &open, flags)) {
        if(open != was_open) {
            node_reset(env, p_open, new_node_bool(open));
        }
        node_idx_t ret = NIL_NODE;
        for(; it; ++it) {
            ret = eval_node(env, *it);
        }
        ImGui::End();
        return ret;
    }
    ImGui::End();
    return NIL_NODE;
}

static node_idx_t native_imgui_input_text(env_ptr_t env, list_ptr_t args) {
    list_t::iterator it(args);
    jo_string label = get_node_string(*it++);
    node_idx_t buf = *it++;
    if(get_node_type(buf) != NODE_ATOM) {
        warnf("imgui/input-text: buf must be an atom\n");
        return NIL_NODE;
    }
    int flags = 0;
    for(; it; ++it) {
        jo_string str = get_node_string(*it);
        if(str == "none")                   flags |= ImGuiInputTextFlags_None;
        else if(str == "chars-decimal")     flags |= ImGuiInputTextFlags_CharsDecimal;
        else if(str == "chars-hexadecimal") flags |= ImGuiInputTextFlags_CharsHexadecimal;
        else if(str == "chars-uppercase")   flags |= ImGuiInputTextFlags_CharsUppercase;
        else if(str == "chars-no-blank")    flags |= ImGuiInputTextFlags_CharsNoBlank;
        else if(str == "auto-select-all")   flags |= ImGuiInputTextFlags_AutoSelectAll;
        else if(str == "enter-returns-true")flags |= ImGuiInputTextFlags_EnterReturnsTrue;
        else if(str == "callback-completion")  flags |= ImGuiInputTextFlags_CallbackCompletion;
        else if(str == "callback-history")  flags |= ImGuiInputTextFlags_CallbackHistory;
        else if(str == "callback-always")   flags |= ImGuiInputTextFlags_CallbackAlways;
        else if(str == "callback-char-filter")  flags |= ImGuiInputTextFlags_CallbackCharFilter;
        else if(str == "allow-tab-input")   flags |= ImGuiInputTextFlags_AllowTabInput;
        else if(str == "ctrl-enter-for-new-line")  flags |= ImGuiInputTextFlags_CtrlEnterForNewLine;
        else if(str == "no-horizontal-scroll")  flags |= ImGuiInputTextFlags_NoHorizontalScroll;
        else if(str == "always-overwrite")  flags |= ImGuiInputTextFlags_AlwaysOverwrite;
        else if(str == "readonly")          flags |= ImGuiInputTextFlags_ReadOnly;
        else if(str == "password")          flags |= ImGuiInputTextFlags_Password;
        else if(str == "no-undo-redo")      flags |= ImGuiInputTextFlags_NoUndoRedo;
        else if(str == "chars-scientific")  flags |= ImGuiInputTextFlags_CharsScientific;
        else if(str == "callback-resize")   flags |= ImGuiInputTextFlags_CallbackResize;
        else if(str == "callback-edit")     flags |= ImGuiInputTextFlags_CallbackEdit;
    }
    jo_string str = get_node_string(node_deref(env, buf));
    char *buf2 = (char*)alloca(256);
    memcpy(buf2, str.c_str(), str.size);
    if(ImGui::InputText(label.c_str(), buf2, 256, flags)) {
        node_reset(env, buf, new_node_string(buf2));
    }
    return NIL_NODE;
}

static node_idx_t native_imgui_button(env_ptr_t env, list_ptr_t args) {
    list_t::iterator it(args);
    jo_string label = get_node_string(eval_node(env, *it++));
    float width = get_node_float(eval_node(env, *it++));
    float height = get_node_float(eval_node(env, *it++));
    node_idx_t ret = NIL_NODE;
    if(ImGui::Button(label.c_str(), ImVec2(width, height))) {
        while(it) {
            ret = eval_node(env, *it++);
        }
    }
    return ret;
}

static node_idx_t native_imgui_image(env_ptr_t env, list_ptr_t args) {
    list_t::iterator it(args);
    long long tex_id = get_node_int(*it++);
    float width = get_node_float(*it++);
    float height = get_node_float(*it++);
    ImGui::Image((void*)tex_id, ImVec2(width, height));
    return NIL_NODE;
}

static node_idx_t native_imgui_text(env_ptr_t env, list_ptr_t args) {
    list_t::iterator it(args);
    jo_string text = get_node_string(eval_node(env, *it++));
    ImGui::TextUnformatted(text.c_str());
    return NIL_NODE;
}

void jo_basic_imgui_init(env_ptr_t env) {
	env->set("imgui/main-menu-bar", new_node_native_function("imgui/main-menu-bar", &native_imgui_main_menu_bar, true, NODE_FLAG_PRERESOLVE));
	env->set("imgui/menu-bar", new_node_native_function("imgui/menu-bar", &native_imgui_menu_bar, true, NODE_FLAG_PRERESOLVE));
	env->set("imgui/menu", new_node_native_function("imgui/menu", &native_imgui_menu, true, NODE_FLAG_PRERESOLVE));
	env->set("imgui/menu-item", new_node_native_function("imgui/menu-item", &native_imgui_menu_item, true, NODE_FLAG_PRERESOLVE));
	env->set("imgui/set-next-window-pos", new_node_native_function("imgui/set-next-window-pos", &native_imgui_set_next_window_pos, false, NODE_FLAG_PRERESOLVE));
	env->set("imgui/set-next-window-size", new_node_native_function("imgui/set-next-window-size", &native_imgui_set_next_window_size, false, NODE_FLAG_PRERESOLVE));
	env->set("imgui/begin", new_node_native_function("imgui/begin", &native_imgui_begin, true, NODE_FLAG_PRERESOLVE));
	env->set("imgui/input-text", new_node_native_function("imgui/input-text", &native_imgui_input_text, false, NODE_FLAG_PRERESOLVE));
	env->set("imgui/button", new_node_native_function("imgui/button", &native_imgui_button, true, NODE_FLAG_PRERESOLVE));
	env->set("imgui/image", new_node_native_function("imgui/image", &native_imgui_image, false, NODE_FLAG_PRERESOLVE));
	env->set("imgui/text", new_node_native_function("imgui/text", &native_imgui_text, false, NODE_FLAG_PRERESOLVE));
}

