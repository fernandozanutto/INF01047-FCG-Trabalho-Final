#pragma once

class Command {
protected:
    bool repeatable = true;
public:
    bool isRepeatable() { return repeatable; };
    virtual void onPress() = 0;
    virtual void onRelease() = 0;
};