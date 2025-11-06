#pragma once

class IntegrationActionBase 
{
public:
    virtual ~IntegrationActionBase() = default;

    virtual void execute() = 0;
};