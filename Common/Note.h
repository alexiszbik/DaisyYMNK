/*
  ==============================================================================

    Note.h
    Created: 17 Jan 2024 8:57:54am
    Author:  Alexis ZBIK

  ==============================================================================
*/

#pragma once

struct Note {

    int pitch = 60;
    int velocity = 0;
    unsigned long timeStamp = 0;

    Note(int pitch, int velocity, unsigned long timeStamp) : pitch(pitch), velocity(velocity), timeStamp(timeStamp) {}
    Note(const Note& a) : pitch(a.pitch), velocity(a.velocity), timeStamp(a.timeStamp) {}
    Note() {}
    /*
    bool operator == (Note const& a) {
        return true;
    }*/
};

