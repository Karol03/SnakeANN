#pragma once

#include "stage.h"

class State
{
public:
    enum Type
    {
        TDrawingState,
        TControlState,
        TValidationState
    };

    State(Type type, Stage& stage)
        : type(type)
        , isFinished_(false)
        , stage_(stage)
    {}
    virtual ~State() = default;

    virtual void execute() = 0;
    virtual Type next() = 0;
    virtual bool isFinished() const final
    { return isFinished_; }

    const Type type;
protected:
    bool isFinished_;
    Stage& stage_;
};
