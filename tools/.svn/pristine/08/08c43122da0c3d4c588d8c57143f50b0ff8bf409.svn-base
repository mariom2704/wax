// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include "ReplacementStatistics.h"



ReplacementStatistics::~ReplacementStatistics()
{
	// ToDo: Add your specialized code here and/or call the base class
}

ReplacementStatistics::ReplacementStatistics() : ReplaceCounter(0), RenamedFileCounter(0), FileCounter(0)
{
	// ToDo: Add your specialized code here and/or call the base class
}

const int ReplacementStatistics::get_FileCounter() const
{
	return FileCounter;
}

void ReplacementStatistics::set_FileCounter(int value)
{
	FileCounter = value;
	return;
}

const int ReplacementStatistics::get_ReplaceCounter() const
{
	return ReplaceCounter;
}

void ReplacementStatistics::set_ReplaceCounter(int value)
{
	ReplaceCounter = value;
	return;
}

const int ReplacementStatistics::get_RenamedFileCounter() const
{
	return RenamedFileCounter;
}

void ReplacementStatistics::set_RenamedFileCounter(int value)
{
	RenamedFileCounter = value;
	return;
}



void ReplacementStatistics::display()
{
	CString msg;
	msg.Format("Es wurden %i Änderungen in %i Dateien vorgenommen!\n%i Dateien wurden umbenamst.", get_ReplaceCounter(), get_FileCounter(), get_RenamedFileCounter());

	AfxMessageBox(msg, MB_OK | MB_ICONEXCLAMATION);
}



void ReplacementStatistics::init()
{
	// hier nicht ändern, Code wird generiert
	set_FileCounter(0);
	set_RenamedFileCounter(0);
	set_ReplaceCounter(0);

}

