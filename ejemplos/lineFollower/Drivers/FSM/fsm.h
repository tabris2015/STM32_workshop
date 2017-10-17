#ifndef FSM_H
#define FSM_H
#include "stm32f1xx_hal.h" // hal libraries
#define MAX_TRANSITIONS 100
#define MAX_STATES 10

typedef bool (*EventHandler)(void);

typedef void (*StateAction)(void);

class Transition
{
public:
    Transition(EventHandler EvtFcn, uint8_t next)
    : EventFcn(EvtFcn), NextState(next){};
    
    EventHandler EventFcn;
    uint8_t NextState;
};

class State
{
public:
    State() {};
    State(StateAction action, Transition * trans, uint8_t t_size, uint32_t timeout)
    :Action(action), 
    Entry(NULL), 
    Exit(NULL), 
    Update(NULL), 
    Trans(trans), 
    t_size(t_size), 
    Timeout(timeout){};

    State(StateAction action, Transition * trans, uint8_t t_size)
    :Action(action), 
    Entry(NULL), 
    Exit(NULL), 
    Update(NULL), 
    Trans(trans), 
    t_size(t_size), 
    Timeout(0){};

    // under development
    State(StateAction entry,
            StateAction exit,
            StateAction update,
            Transition * trans, 
            uint8_t t_size, 
            uint32_t timeout)
    :Action(NULL), 
    Entry(entry), 
    Exit(exit), 
    Update(update), 
    Trans(trans), 
    t_size(t_size), 
    Timeout(timeout){};

    State(StateAction entry,
            StateAction exit,
            StateAction update,
            Transition * trans, 
            uint8_t t_size)
    :Action(NULL), 
    Entry(entry), 
    Exit(exit), 
    Update(update), 
    Trans(trans), 
    t_size(t_size), 
    Timeout(0){};

    // ----
    StateAction Action;
    StateAction Entry;
    StateAction Exit;
    StateAction Update;
    Transition * Trans;
    uint8_t t_size;
    uint32_t Timeout;
};


class StateMachine
{
public:
    StateMachine(State * states, uint8_t initState, bool enable);
    StateMachine(State * states, uint8_t initState);
    void UpdateState();
    void Run();
    bool StateChanged;
    bool Elapsed;
    
private:
    bool Enabled;
    uint32_t TimeToExit;
    uint8_t CurrentState;
    
    State * states;
    Transition * trans;
    uint32_t prevTime;
    
};



#endif  //FSM_H
