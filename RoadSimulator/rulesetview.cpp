#include "rulesetview.h"
#include "config.h"

#include <fstream>

#define STB_TRUETYPE_IMPLEMENTATION
namespace Stb_TrueType {
	#include <stb_truetype/stb_truetype.h>
}

RuleSetView::RuleSetView(float left, float top)
    : left(left), top(top)
{
    fontsize = (float)Config::get().data.MESSAGES_FONT_SIZE;
    historySize = Config::get().data.MESSAGES_HISTORY_SIZE;
    initFont("C:\\Windows\\Fonts\\couri.ttf");
}

RuleSetView::~RuleSetView()
{}

void RuleSetView::draw()
{
    int ypos = 0;
    for (auto i=infoText.rbegin(); i!=infoText.rend(); i++)
    {
        drawText(left, top + ypos*fontsize, *i);
        ypos++;
    }
}

void RuleSetView::update(RuleSet *ev)
{
    RuleSet::ViolationType v = ev->getViolation();
    switch(v) {
    case RuleSet::WRONG_DIRECTION:
        infoText.push_back("[violation] Wrong direction");
        break;
    case RuleSet::SOLID_LINE_CROSS:
        infoText.push_back("[violation] Solid line crossing");
        break;
    case RuleSet::OK:
        infoText.push_back("[info] Ok");
        break;
    case RuleSet::TURN_MANEUR_WARN:
        infoText.push_back("[warn] Turn maneur begin");
        break;
    case RuleSet::TURN_MANEUR_VIOL:
        infoText.push_back("[violation] Bad turn maneur");
        break;
    case RuleSet::TURN_MANEUR_OK:
        infoText.push_back("[info] Successfull turn maneur");
        break;
    case RuleSet::NOT_IN_ROAD:
        infoText.push_back("[violation] Not in road. Return back!");
        break;
    }
    while (infoText.size() > historySize) {
        infoText.pop_front();
    }
}

void RuleSetView::initFont(char* filename)
{
    using namespace Stb_TrueType;
    char* ttf_fileBuffer;

    std::ifstream file(filename, std::ios::binary);
	if (!file.is_open())
		throw FontFileNotFound();
    file.seekg(0, std::ios::end);
    unsigned long int ttf_fileBufferSize = (unsigned long int)file.tellg();
    file.seekg(0, std::ios::beg);

    ttf_fileBuffer = new char[ttf_fileBufferSize];
	if (! file.read(ttf_fileBuffer, ttf_fileBufferSize)) throw FontFileNotFound();

    cdata = new stbtt_bakedchar[96];
    unsigned char *temp_bitmap = new unsigned char[512*512];

    stbtt_BakeFontBitmap(reinterpret_cast<const unsigned char*>(ttf_fileBuffer), 0,
                            fontsize,
                            temp_bitmap,512,512,
                            32,96,
                            reinterpret_cast<stbtt_bakedchar*>(cdata));
    delete[] ttf_fileBuffer;


    glGenTextures(1, &ftex);
    glBindTexture(GL_TEXTURE_2D, ftex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    delete[] temp_bitmap;
}

void RuleSetView::drawText(float x, float y, std::string& text)
{
    using namespace Stb_TrueType;
    y += fontsize;
    glBindTexture(GL_TEXTURE_2D, ftex);
    glBegin(GL_QUADS);
    for (unsigned int i=0; i<text.size(); i++) {
        if (text[i] >= 32) { //&& text[i] < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(reinterpret_cast<stbtt_bakedchar*>(cdata), 
				512,512, text[i]-32, &x,&y,&q,1);
            glTexCoord2f(q.s0,q.t1); glVertex2f(q.x0,q.y1);
            glTexCoord2f(q.s1,q.t1); glVertex2f(q.x1,q.y1);
            glTexCoord2f(q.s1,q.t0); glVertex2f(q.x1,q.y0);
            glTexCoord2f(q.s0,q.t0); glVertex2f(q.x0,q.y0);
        }
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}
