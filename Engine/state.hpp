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
        , isCloseWindowGame_(false)
        , stage_(stage)
    {}
    virtual ~State() = default;

    virtual void execute() = 0;
    virtual Type next() = 0;
    virtual bool isFinished() const final
    { return isFinished_; }
    virtual bool isCloseWindowGame() const final
    { return isCloseWindowGame_; }

    const Type type;

protected:
    bool isCloseWindowGame_;
    Stage& stage_;
    static bool isFinished_;
};
