#pragma once

#include <data/IDoc.h>
#include <arrays/Enumerator.h>
#include <data/IMoveDoc.h>
#include "CowPath.h"

class ICowFootprint
{
public:
	virtual IDoc* GetDoc() const = 0;
	virtual ICowEntry* GetEntry() const = 0;
};

class ICowMovement
{
public:
	virtual IMoveDoc* GetDoc() const = 0;
	virtual ICowEntry* GetEntry() const = 0;
};

class ICowTrail
{
public:
	virtual const ICowFootprint* GetEntry() const = 0;
	virtual const ICowFootprint* GetExit() const = 0;	
	virtual PtrEnumerator<ICowMovement> GetTrail() = 0;
	virtual bool IsStationary() const = 0;
	virtual CowPath GetPath() = 0;
};
