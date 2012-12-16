/**
 * @file factory.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2011
 */

#ifndef _FACTORY_H_
#define _FACTORY_H_

class IDiagramMode;

template <class _Class>
class IFactory {
public:
    virtual ~IFactory() { }
    virtual _Class* create() = 0;
};

class IModeFactory : public IFactory<IDiagramMode> {
public:
    virtual IDiagramMode* create() = 0;
};

template <class _Class>
class ModeFactory : public IModeFactory {
public:
    virtual IDiagramMode* create() { return new _Class(); }
};

#endif // _FACTORY_H_
