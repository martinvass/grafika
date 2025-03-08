int fontsize = 24;
int t_width = 0; // width of the loaded font-texture
int t_height = 0; // height of the loaded font-texture
SDL_Color text_color = {0,0,0};
string fontpath = "my font path";
string text = "text I want to display";
TTF_Font* font = TTF_OpenFont(fontpath.c_str(), fontsize);
SDL_Texture* ftexture = NULL; // our font-texture

// check to see that the font was loaded correctly
if (font == NULL) {
    cerr << "Failed the load the font!\n";
    cerr << "SDL_TTF Error: " << TTF_GetError() << "\n";
}
else {
    // now create a surface from the font
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, text.c_str(), text_color);

    // render the text surface
    if (text_surface == NULL) {
        cerr << "Failed to render text surface!\n";
        cerr << "SDL_TTF Error: " << TTF_GetError() << "\n";
    }
    else {
        // create a texture from the surface
        ftexture = SDL_CreateTextureFromSurface(renderer, text_surface);

        if (ftexture == NULL) {
            cerr << "Unable to create texture from rendered text!\n";
        }
        else {
            t_width = text_surface->w; // assign the width of the texture
            t_height = text_surface->h; // assign the height of the texture

            // clean up after ourselves (destroy the surface)
            SDL_FreeSurface(surface);
        }
    }
}