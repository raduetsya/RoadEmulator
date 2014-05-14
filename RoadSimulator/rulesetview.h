#ifndef RULESETVIEW_H
#define RULESETVIEW_H

#include "ruleset.h"
#include "iview.h"
#include "observer.h"

#include <windows.h>
#include <GL/gl.h>

#include <string>
#include <list>

class RuleSetView :
        public Observer<RuleSet>,
        public IView
{
public:
    RuleSetView(float left, float top);
    void draw();
    void update(RuleSet *ev);

	class FontFileNotFound : std::exception {};

private:
    float left, top, fontsize;
    unsigned int historySize;
    RuleSet::ViolationType viol;
    std::list<std::string> infoText;

// stb_tt font rendering library
    GLuint ftex;
    void *cdata; // backed ASCII 32..126 is 95 glyphs

    void initFont(char* filename);
    void drawText(float x, float y, std::string& text);
};

#endif // RULESETVIEW_H
