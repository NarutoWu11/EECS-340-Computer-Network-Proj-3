#ifndef _messages
#define _messages

#include <iostream>

#include "node.h"
#include "link.h"

#if defined(GENERIC)
struct RoutingMessage {
 public:
  ostream & Print(ostream &os) const;
};
#endif

#if defined(LINKSTATE)
struct RoutingMessage {

	RoutingMessage();
	RoutingMessage(const RoutingMessage &rhs);
	RoutingMessage(const vector<vector<Entry> > &, const bool);
	RoutingMessage &operator=(const RoutingMessage &rhs);

	ostream & Print(ostream &os) const;

	vector<vector<Entry> > neighborMessage;
	bool linkChange;

};



#endif

#if defined(DISTANCEVECTOR)
struct RoutingMessage {

	RoutingMessage();
	RoutingMessage(const RoutingMessage &rhs);
	RoutingMessage(const vector<Entry> &);
	RoutingMessage &operator=(const RoutingMessage &rhs);

	ostream & Print(ostream &os) const;

	vector<Entry> neighborMessage;

};
#endif


inline ostream & operator<<(ostream &os, const RoutingMessage &m) { return m.Print(os);}

#endif
