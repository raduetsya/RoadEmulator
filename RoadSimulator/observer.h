#ifndef OBSERVER_H
#define OBSERVER_H

#include <list>

template <class T>
class Observer {
public:
    virtual void update(T* ev) = 0;
};

template <class T>
class Observable
{
public:
    void attach(Observer<T> &obs)
    {
        quietAttach(obs);
        notify();
    }

    void quietAttach(Observer<T> &obs)
    {
        // TODO: check unique
        _observersList.push_back(&obs);
    }

    void notify()
    {
        for (auto i = _observersList.begin();
            i != _observersList.end();
            ++i)
        {
            (*i)->update(static_cast<T*>(this));
        }
    }

private:
    std::list<Observer<T> *> _observersList;
};

#endif // OBSERVER_HPP
