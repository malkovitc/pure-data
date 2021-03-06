//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sat Feb 14 21:55:38 PST 2015
// Last Modified: Sat Feb 14 21:55:40 PST 2015
// Filename:      midifile/include/MidiEventList.h
// Website:       http://midifile.sapp.org
// Syntax:        C++
// vim:           ts=3 expandtab
//
// Description:   A class which stores a MidiEvents for a MidiFile track.
//

#ifndef _MIDIEVENTLIST_H_INCLUDED
#define _MIDIEVENTLIST_H_INCLUDED

#include "MidiEvent.h"
#include <vector>

class MidiEventList {
public:
    MidiEventList(void);

    ~MidiEventList();

    MidiEventList(const MidiEventList& other);
    MidiEventList* clone() const;

    MidiEvent& back(void);
    MidiEvent& last(void);
    MidiEvent& getEvent(int index);
    void clear(void);
    void reserve(int rsize);
    int getSize(void) const;
    int size(void) const;
    int linkNotePairs(void);
    int linkEventPairs(void);
    void clearLinks(void);
    MidiEvent** data(void);

    int push(MidiEvent& event);
    int push_back(MidiEvent& event);
    int append(MidiEvent& event);

    // careful when using these, intended for internal use in MidiFile class:
    void detach(void);
    int push_back_no_copy(MidiEvent* event);

    MidiEventList& operator=(MidiEventList other);

    typedef std::vector<MidiEvent*>::iterator iterator;
    typedef std::vector<MidiEvent*>::const_iterator const_iterator;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    /**
     * Checks if event list is empty
     */
    bool empty() const { return list.empty(); }

    /**
     * Returns event at specified position
     * @note no range checks
     */
    MidiEvent& operator[](size_t index) { return *list[index]; }
    const MidiEvent& operator[](size_t index) const { return *list[index]; }

private:
    std::vector<MidiEvent*> list;
};

#endif /* _MIDIEVENTLIST_H_INCLUDED */
