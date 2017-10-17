#include "fsm.h"

StateMachine::StateMachine(State *sts, uint8_t initState, bool enable) 
{
  Enabled = enable;
  CurrentState = initState;
  StateChanged = true;
  states = sts;
  trans = this->states[CurrentState].Trans;
  TimeToExit = this->states[CurrentState].Timeout;
}
StateMachine::StateMachine(State *sts, uint8_t initState) 
{
  StateMachine(sts, initState, true);
}
void StateMachine::UpdateState() 
{
  Transition *_t = this->trans;
  uint8_t _n = this->states[CurrentState].t_size;

  for (int i = 0; i < _n; i++) 
  {
    if (_t[i].EventFcn()) 
    {
      // if event, meke transition
      // first execute exit action
      if(states[CurrentState].Exit != NULL)
      {
        states[CurrentState].Exit();
      }
      // then update the next state and transitions
      CurrentState = _t[i].NextState;
      trans = states[CurrentState].Trans;
      Elapsed = false;
      TimeToExit = states[CurrentState].Timeout;
      StateChanged = true;
      if(states[CurrentState].Entry != NULL)
      {
        states[CurrentState].Entry();
        StateChanged = false;
      }
      break;
    }
  }
}
void StateMachine::Run() 
{
  if (!Enabled) 
  {
    return;
  }
  if (states[CurrentState].Timeout != 0) 
  {
    uint32_t deltatime = HAL_GetTick() - prevTime;
    prevTime = HAL_GetTick();
    TimeToExit -= deltatime;

    if (TimeToExit <= 0) 
    {
      Elapsed = true;
    }
  }
  UpdateState();
  // update state
  if(states[CurrentState].Update != NULL)
  {
    states[CurrentState].Update();
  }
  //
  states[CurrentState].Action();
}
