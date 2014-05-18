#ifndef IVIEW_H
#define IVIEW_H

class IView
{
public:
    virtual void draw() = 0;
	virtual ~IView() {};
};

#endif // IVIEW_H
