#ifndef _table
#define _table


#include <iostream>

using namespace std;

#if defined(GENERIC)
class Table {
  // Students should write this class

 public:
  ostream & Print(ostream &os) const;
};
#endif




/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/




#if defined(LINKSTATE)
    #include <vector>

    class Entry{
    public:
        Entry(unsigned destNode, unsigned pred, double cost, bool visited);
        Entry();
        Entry(const Entry &);
        unsigned GetNumber() const;
        unsigned GetPred() const;
        double GetCost() const;
        void SetCost(double);
        void SetPred(unsigned);
        void SetVisited(bool);
        bool GetVisited() const;
        
    private:
        unsigned destNode;
        unsigned pred;
        double cost;
        bool visited;
    };


    class Table {
    // Students should write this class
    public:
        Table();
        Table(unsigned);
        Table(const Table &);
        ostream & Print(ostream &os) const;
        void CreateLine(unsigned number); 
        void InsertEntry(unsigned number, unsigned nextHop,double cost,bool visited, vector<Entry> * lineNum);
        vector<Entry> * Get1stLine();
        vector<Entry> * GetLinePlus(unsigned nodeNumber);
        Entry * GetEntryPlus(vector<Entry> * linePtr, unsigned nodeNumber);
        void PrintCerr();
        void DPrintCerr();
        vector<Entry> * ChangeLine(const vector<Entry> &, unsigned number);
        bool LSUpdateTable (const vector<vector<Entry> > *);
        bool LSUpdateLine (const vector<vector<Entry> > *);
        void CalculateDResult();
        void ClearDResult();

        Entry * DGetEntry(unsigned number);
        void DInsertEntry(unsigned, unsigned, double,bool);


        vector<vector<Entry> > * tableData;
        vector<Entry> * dResult;

    };
  

#endif

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

#if defined(DISTANCEVECTOR)
    #include <vector>

    class Entry{
    public:
        Entry(unsigned destNode, unsigned nextHop, double cost);
        Entry();
        Entry(const Entry &);
        unsigned GetNumber() const;
        unsigned GetNextHop() const;
        double GetCost() const;
        void SetCost(double);
        void SetNextHop(unsigned);
        
    private:
        unsigned destNode;
        unsigned nextHop;
        double cost;
    };


    class Table {
    // Students should write this class
    public:
        Table();
        Table(unsigned);
        Table(const Table &);
        ostream & Print(ostream &os) const;
        void CreateLine(unsigned number); 
        void InsertEntry(unsigned number, unsigned nextHop,double cost,unsigned lineNum);
        vector<Entry> * Get1stLine();
        vector<Entry> * GetLinePlus(unsigned nodeNumber);
        Entry * GetEntryPlus(vector<Entry> * linePtr, unsigned nodeNumber);
        void PrintCerr();
        bool UpdateTable(vector<Entry> *);
        void UpdateTable(unsigned, double);
        vector<Entry> * ChangeLine(const vector<Entry> &, unsigned number);


        vector<vector<Entry> > * tableData;
    };


#endif

inline ostream & operator<<(ostream &os, const Table &t) { return t.Print(os);}

#endif
