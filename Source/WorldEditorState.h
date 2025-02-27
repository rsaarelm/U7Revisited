#ifndef _WORLDEDITORSTATE_H_
#define _WORLDEDITORSTATE_H_

#include "Geist/Gui.h"
#include "Geist/State.h"
#include <deque>
#include <list>
#include <math.h>

class WorldEditorState : public State
{
public:
	WorldEditorState() {};
	~WorldEditorState();

	virtual void Init(const std::string& configfile);
	virtual void Shutdown();
	virtual void Update();
	virtual void Draw();

	virtual void OnEnter();
	virtual void OnExit();
};

#endif