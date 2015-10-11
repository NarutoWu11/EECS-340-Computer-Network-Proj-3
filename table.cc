#include "table.h"

#if defined(GENERIC)
	ostream & Table::Print(ostream &os) const
	{
		// WRITE THIS
		os << "Table()";
		return os;
	}
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
    #include <limits>
    Table::Table(){}
    Table::Table(unsigned number){
        //cerr<<"new table added"<<endl;
        tableData = new vector<vector<Entry> >();
        CreateLine(number);

        dResult = new vector<Entry>();
        Entry * newEntry = new Entry(number,number,0,false);
        dResult->push_back(*newEntry);

        //PrintCerr();
    }

    Table::Table(const Table & table){
        tableData = new vector<vector<Entry> >();
        dResult = new vector<Entry>();
        for(int i=0;i<table.tableData->size();i++){
            /*
            vector<Entry>  thisLine();
            for(int j=0;j<(*(table.tableData))[i].size();j++){
                thisLine.push_back((*(table.tableData))[i][j]);
            }
            */
            tableData->push_back((*(table.tableData))[i]);
        }

        for(int i=0;i<table.dResult->size();i++){
            dResult->push_back((*table.dResult)[i]);
        }
    }


    ostream & Table::Print(ostream &os) const
    {
        os <<endl
            <<"LinkState Table (Adjacency Table): "<<endl
            <<"src node:"<<" destination nodes: "<<endl;
        for(int i=0; i<tableData->size();i++){
            for(int j=0; j<(*tableData)[i].size(); j++){
                os<<(*tableData)[i][j].GetNumber()<<"/"<<(*tableData)[i][j].GetPred()<<"/"<<(*tableData)[i][j].GetCost()<<" ";
            }
            os<<endl;
        }
        os<<"Dijkstra Result: "<<endl;
        for(int i=0; i<dResult->size();i++){
            os<<(*dResult)[i].GetNumber()<<"/"<<(*dResult)[i].GetPred()<<"/"<<(*dResult)[i].GetCost()<<" ";
        }
        os<<endl;
        return os;
    }



    void Table::PrintCerr(){
        //cerr<<"print: "<<tableData<<endl;
        for(int i=0;i<tableData->size();i++){
            for(int j=0;j<(*tableData)[i].size();j++){
                cerr<<(*tableData)[i][j].GetNumber()<<"/"<<(*tableData)[i][j].GetPred()<<"/"<<(*tableData)[i][j].GetCost()<<" ";
            }
            cerr<<endl;
        }
    }

    void Table::DPrintCerr(){
        for(int j=0;j<dResult->size();j++){
            cerr<<(*dResult)[j].GetNumber()<<"/"<<(*dResult)[j].GetPred()<<"/"<<(*dResult)[j].GetCost()<<" ";
        }
        cerr<<endl;
    }

    
    void Table::CreateLine(unsigned number){
        vector<Entry>  *thisLine = new vector<Entry>();
        Entry * thisEntry = new Entry(number,0,0,false);
        thisLine->push_back(*thisEntry);
        tableData->push_back(*thisLine);
    }


    vector<Entry> * Table::GetLinePlus(unsigned nodeNumber){
        for(int lineNumber = 0;lineNumber < tableData->size();lineNumber++){
            if((*tableData)[lineNumber][0].GetNumber() == nodeNumber){
                return & ((*tableData)[lineNumber]);
            }
        }
        return NULL;
    }

    vector<Entry> * Table::Get1stLine(){
        return & ((*tableData)[0]);
    }

    void Table::InsertEntry(unsigned number, unsigned nextHop, double cost, bool visited, vector<Entry> * linePtr){
        Entry  * newEntry = new Entry(number,nextHop,cost,visited);
        linePtr->push_back(*newEntry);
    }

    Entry * Table::GetEntryPlus(vector<Entry> * linePtr, unsigned nodeNumber){
        for(int i=0;i<(*linePtr).size();i++){
            if((*linePtr)[i].GetNumber() == nodeNumber){
                //cerr<<(*linePtr)[i].GetNumber()<<endl;
                return &((*linePtr)[i]);
            }
            
        }
        return NULL;
    }



    vector<Entry> * Table::ChangeLine(const vector<Entry> & neighborMessaages, unsigned number){
        for(int i=0;i<tableData->size();i++){
            if ((*tableData)[i][0].GetNumber() == number){
                tableData->erase(tableData->begin()+i);
            }
        }

        tableData->push_back(neighborMessaages);
        return &((*tableData)[tableData->size()-1]);
    }


    

    bool Table::LSUpdateTable (const vector<vector<Entry> > * newTable){
        vector<Entry> * linePtr;
        const Entry * currentEntry;
        bool changed = false;
        bool lineFound = false;
        //fisrt line of the new coming table handle
        for (int k=0;k<newTable->size();k++){
            lineFound = false;
            for (int  i= 1; i < tableData->size(); i++)
            {   
                linePtr = &((*tableData)[i]);
                if ((*tableData)[i][0].GetNumber() == (*newTable)[k][0].GetNumber()){
                    lineFound = true;
                    //cerr<<"found line: "<<k<<endl;
                    for (int j=0;j<(*newTable)[k].size();j++){
                        currentEntry = &((*newTable)[k][j]);
                        if (GetEntryPlus(linePtr, currentEntry->GetNumber()) == NULL){
                            InsertEntry(currentEntry->GetNumber(),(*tableData)[i][0].GetNumber(),currentEntry->GetCost(),false,linePtr);
                            changed = true;
                        }
                        /*
                        else if (GetEntryPlus(linePtr, currentEntry->GetNumber())->GetCost() 
                                != (*newTable)[k][j].GetCost()){
                            GetEntryPlus(linePtr, currentEntry->GetNumber())->SetCost((*newTable)[k][j].GetCost());
                            changed=true;
                        }
                        */
                    }
                    break;
                }
            }
            if (lineFound == false && (*newTable)[k][0].GetNumber() != (*tableData)[0][0].GetNumber()){
                linePtr = new vector<Entry>();
                for(int j=0; j<(*newTable)[k].size();j++){
                    currentEntry = new Entry((*newTable)[k][j]);
                    linePtr->push_back(*currentEntry);
                }
                tableData->push_back(*linePtr);
                changed = true;
            }
        }
        return changed;
    }

    bool Table::LSUpdateLine (const vector<vector<Entry> > * newTable){
        bool changed = false;
        bool lineFound = false;
        const Entry * entryPtr;
        const Entry * currentEntry1;
        const Entry * currentEntry2;
        vector<Entry> * linePtr;

        currentEntry1 = &((*newTable)[0][0]);

        currentEntry2 = &((*newTable)[0][1]);

        for (int  i= 0; i < tableData->size(); i++){
            linePtr = &((*tableData)[i]);
            if((*tableData)[i][0].GetNumber() == currentEntry1->GetNumber()){
                if (GetEntryPlus(linePtr, currentEntry2->GetNumber()) == NULL){
                    InsertEntry(currentEntry2->GetNumber(),currentEntry1->GetNumber(),currentEntry2->GetCost(),false,linePtr);
                    if (i == 0){
                        CreateLine(currentEntry2->GetNumber());
                    }
                    changed = true;
                }
                else if (GetEntryPlus(linePtr, currentEntry2->GetNumber())->GetCost() 
                        != currentEntry2->GetCost()){
                    GetEntryPlus(linePtr, currentEntry2->GetNumber())->SetCost(currentEntry2->GetCost());
                    changed=true;
                }
                lineFound = true;
                break;
            }
        }
        if (lineFound == false && (*newTable)[0][0].GetNumber() != (*tableData)[0][0].GetNumber()){
            linePtr = new vector<Entry>();
            
            entryPtr = new Entry(currentEntry1->GetNumber(),currentEntry1->GetNumber(),0,false);
            linePtr->push_back(*currentEntry1);
            entryPtr = new Entry(currentEntry2->GetNumber(),currentEntry1->GetNumber(),currentEntry2->GetCost(),false);
            linePtr->push_back(*currentEntry2);
            
            
            tableData->push_back(*linePtr);
            changed = true;
        } 
        return changed;
    }


    Entry * Table::DGetEntry(unsigned number)
    {
        for(int i=0;i<dResult->size();i++){
            if((*dResult)[i].GetNumber() == number){
                //cerr<<(*dResult)[i].GetNumber()<<endl;
                return &((*dResult)[i]);
            }
        }
        return NULL;
    }

    void Table::DInsertEntry(unsigned n, unsigned pd, double c, bool v){
        Entry * newEntry = new Entry(n,pd,c,v);
        //Entry newEntry(n,pd,c,v);
        dResult->push_back( * newEntry);

    }


    void Table::ClearDResult(){
        //cerr << "clear last dResult!" <<endl;
        unsigned selfNum = (* tableData)[0][0].GetNumber();
        delete dResult;
        Entry * newEntry = new Entry(selfNum, selfNum, 0, false);
        dResult = new vector<Entry>;
        dResult->push_back( *newEntry);
    }

    void Table::CalculateDResult(){
        //clear old dResult
        //cerr<<"table before dcal: "<<endl;
        //PrintCerr();
        ClearDResult();
        //cerr<<"dreault after clear"<<endl;
        //DPrintCerr();
        vector<Entry> * frontier = Get1stLine();
        unsigned frontNum = (* tableData)[0][0].GetNumber();
        double tempCost = 0;
        double frontCost = 0;
        //unsigned newFrontierNum = 0;
        //unsigned tempNum = 0;
        Entry * changeEntry = NULL;
        //Entry * backUpFrontier = &(* dResult)[0];

        //initialize dResult
        for (int i = 1; i <  tableData->size(); i++){
            unsigned destNum = (* tableData)[i][0].GetNumber();
            DInsertEntry(destNum, destNum, std::numeric_limits<double>::infinity(), false);             
        }
        //for (int j = 0; j < (* dResult).size(); j++){

        //DPrintCerr();
        for (;;){
             //set frontier neighbor values
            for (int k = 1; k < (* frontier).size(); k++){
                changeEntry = DGetEntry((* frontier)[k].GetNumber());
                if (changeEntry == NULL){
                    // new node to be add
                    DInsertEntry((* frontier)[k].GetNumber(), (* frontier)[0].GetNumber(),frontCost + (* frontier)[k].GetCost(), false);
                } 
                else if ((*DGetEntry(changeEntry->GetNumber())).GetVisited() == false) {
                    if ((frontCost + (* frontier)[k].GetCost()) < changeEntry->GetCost()){  
                        changeEntry->SetCost(frontCost + (* frontier)[k].GetCost());
                        changeEntry->SetPred((* frontier)[0].GetNumber());
                    }
                }
            }
            //mark visited
            (* DGetEntry(frontNum)).SetVisited(true);
            frontNum = (* tableData)[0][0].GetNumber();
            //linkstate algorithm to get frontier
            tempCost = std::numeric_limits<double>::infinity();
            for (int m = 1; m < (* dResult).size(); m++){
                if (((* dResult)[m].GetCost()  < tempCost) && ((* dResult)[m].GetVisited() != true)){
                    tempCost = (* dResult)[m].GetCost(); 
                    frontNum = (* dResult)[m].GetNumber();                    
                }
            }
            if ((frontNum == (* tableData)[0][0].GetNumber()) || (GetLinePlus(frontNum) == NULL)){
                //cerr << "there is no valid frontier neighbor message!" << endl;
                //DPrintCerr();
                return;
            }
                
            frontier = GetLinePlus(frontNum);
            frontCost = tempCost;
        }
          
    }



    

    
    Entry::Entry(){}

    Entry::Entry(unsigned dN, unsigned pd, double c, bool v){
        cost = c;
        destNode = dN;
        pred = pd;
        visited = v;
    }

    Entry::Entry(const Entry (& e)):
    cost(e.cost), destNode(e.destNode), pred(e.pred), visited(e.visited)
    {
        //SetCost(e.GetCost());
        //destNode = e.GetNumber();
        //nextHop = e.GetNextHop();
    }

    unsigned Entry::GetNumber() const{
        return destNode;
    }

    unsigned Entry::GetPred() const{
        return pred;
    }

    double Entry::GetCost() const{
        return cost;
    }

    void Entry::SetCost(double c){
        cost = c;
    }

    void Entry::SetPred(unsigned pd){
        pred = pd;
    }

    void Entry::SetVisited(bool v){
        visited = v;
    }

    bool Entry::GetVisited() const{
        return visited;
    }
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
	Table::Table(){}
	Table::Table(unsigned number){
		//cerr<<"new table added"<<endl;
		tableData = new vector<vector<Entry> >();
		CreateLine(number);
		//PrintCerr();
	}

	Table::Table(const Table & table){
		tableData = new vector<vector<Entry> >;
		for(int i=0;i<table.tableData->size();i++){
			/*
			vector<Entry>  thisLine();
			for(int j=0;j<(*(table.tableData))[i].size();j++){
				thisLine.push_back((*(table.tableData))[i][j]);
			}
			*/
			tableData->push_back((*(table.tableData))[i]);
		}
	}

    ostream & Table::Print(ostream &os) const
    {
        os <<endl
            <<"Distance Vector Table: (the first line is the correct distance from src node)"<<endl
            <<"src node:"<<" destination nodes: "<<endl;
        for(int i=0; i<tableData->size();i++){
            for(int j=0; j<(*tableData)[i].size(); j++){
                os<<(*tableData)[i][j].GetNumber()<<"/"<<(*tableData)[i][j].GetNextHop()<<"/"<<(*tableData)[i][j].GetCost()<<" ";
            }
            os<<endl;
        }
        return os;
    }

	void Table::PrintCerr(){
		//cerr<<"print: "<<tableData<<endl;
		for(int i=0;i<tableData->size();i++){
			for(int j=0;j<(*tableData)[i].size();j++){
				cerr<<(*tableData)[i][j].GetNumber()<<"/"<<(*tableData)[i][j].GetNextHop()<<"/"<<(*tableData)[i][j].GetCost()<<" ";
			}
			cerr<<endl;
		}
	}

	
	void Table::CreateLine(unsigned number){
		vector<Entry>  *thisLine = new vector<Entry>();
		Entry thisEntry(number,0,0);
		thisLine->push_back(thisEntry);
		tableData->push_back(*thisLine);
	}


	vector<Entry> * Table::GetLinePlus(unsigned nodeNumber){
		for(int lineNumber = 0;lineNumber < tableData->size();lineNumber++){
			if((*tableData)[lineNumber][0].GetNumber() == nodeNumber){
				return & ((*tableData)[lineNumber]);
			}
		}
		return NULL;
	}

	vector<Entry> * Table::Get1stLine(){
		return & ((*tableData)[0]);
	}

	void Table::InsertEntry(unsigned number, unsigned nextHop, double cost, unsigned lineNum){
		Entry  newEntry(number,nextHop,cost);
		(*tableData)[lineNum].push_back(newEntry);
	}

	Entry * Table::GetEntryPlus(vector<Entry> * linePtr, unsigned nodeNumber){
		for(int i=0;i<(*linePtr).size();i++){
			if((*linePtr)[i].GetNumber() == nodeNumber){
				//cerr<<(*linePtr)[i].GetNumber()<<endl;
				return &((*linePtr)[i]);
			}
			
		}
		return NULL;
	}


	vector<Entry> * Table::ChangeLine(const vector<Entry> & neighborMessaages, unsigned number)
    {
        for(int i=0;i<tableData->size();i++){
        	if ((*tableData)[i][0].GetNumber() == number){
        		tableData->erase(tableData->begin()+i);
        	}
        }

        tableData->push_back(neighborMessaages);
        return &((*tableData)[tableData->size()-1]);
    }


	bool Table::UpdateTable(vector<Entry> *changedNeighbor){
		bool change = false;
		//cerr<<"update table method 1:"<<endl;
        unsigned neighborNum = (* changedNeighbor)[0].GetNumber();
        //cerr<<neighborNum<<endl;
        unsigned neighborDestNum = 0;
        Entry * neighborDest = NULL;
        //cerr<<(*Get1stLine())[1].GetNumber()<<endl;
        double linkCost = (* GetEntryPlus(Get1stLine(), neighborNum)).GetCost();

        double tempCost = 0;
        Entry * dest = NULL;
        for(int i = 1; i < (* changedNeighbor).size(); i++) {
            neighborDest = &((* changedNeighbor)[i]);
            neighborDestNum = neighborDest->GetNumber();
            // 1st line has the destination in changedNeighbor
            if (( dest =  GetEntryPlus(Get1stLine(), neighborDestNum)) != NULL){
                // distance from changedNeighbor to destination is shorter
                if ((* dest).GetCost() >  (tempCost = linkCost + (* neighborDest).GetCost())){
                	change = true;
                    (* dest).SetCost(tempCost);
                    (* dest).SetNextHop(neighborNum);
                }
            }
            // 1st line doesn't have the destination in changedNeighbor
            else{
            	change = true;
                Entry * newDest = new Entry(neighborDestNum, neighborNum, neighborDest->GetCost() + linkCost);
                (* Get1stLine()).push_back(*newDest);
            }
        }
        return change;
    }

    void Table::UpdateTable(unsigned changedNeighbor, double changedCost) {
    	//bool change =false;
        Entry * changedEntry =  GetEntryPlus(Get1stLine(), changedNeighbor);
        vector<Entry> * neighborVector = NULL;
        unsigned neighborDestNum = 0;
        unsigned next = 0;
        Entry * neighborDest = NULL;
        //double linkCost = (* GetEntryPlus(Get1stLine(), changedNeighbor)).GetCost();
        double tempCost = 0;
        double newCost = 0;
        double compareCost = 0;
        Entry * dest = NULL;
        Entry * compare1 = NULL;
        Entry * compare2 = NULL;
        // changedCost is larger than original distance and if distance is indirect, return 
        if (changedCost >= changedEntry->GetCost() && changedEntry->GetNextHop() != changedNeighbor){
            //cerr << "No need to change because link length is longer!" << endl;
            return;
        }
        // changedCost is smaller than original distance
        double oldCost = changedEntry->GetCost();
        changedEntry->SetCost(changedCost);
        for (int i = 1; i < (* tableData).size(); i++){
            compare1 = GetEntryPlus(&(* tableData)[i], changedNeighbor);
            compare2 = GetEntryPlus(Get1stLine(), (* tableData)[i][0].GetNumber());
            if (compare1 != NULL && (compareCost = compare1->GetCost() + compare2->GetCost()) < changedCost) {
                changedCost = compareCost;
                next = (* tableData)[i][0].GetNumber(); 
            }
                                
        }
        changedEntry->SetNextHop(next);
        changedEntry->SetCost(changedCost);

        //clear variables
        next = 0;
        compare1 = NULL;
        compare2 = NULL;
        compareCost = 0;
        // get the vector of changedNeighbor
        neighborVector = GetLinePlus(changedNeighbor);
        
 
           
        for(int i = 1; i < (* neighborVector).size(); i++){
            neighborDest = &((* neighborVector)[i]);
            neighborDestNum = neighborDest->GetNumber();
            tempCost = changedCost + (* neighborDest).GetCost(); //need to change
            dest =  GetEntryPlus(Get1stLine(), neighborDestNum);
            // next hop isn't changedNeighbor
            if ((* dest).GetNextHop() != changedNeighbor){
                if ((* dest).GetCost() > tempCost){
                        (* dest).SetCost(tempCost);
                        (* dest).SetNextHop(changedNeighbor);
                }
            } 
            // next hop is changedNeighbor and cost is larger
            else if (changedCost > oldCost){
                next = changedNeighbor;
                for(int j = 1; j < (* tableData).size(); j++){
                    compare1 = GetEntryPlus(&(* tableData)[j], neighborDestNum);
                    compare2 = GetEntryPlus(Get1stLine(), (* tableData)[j][0].GetNumber());
                    
                    if (compare1!= NULL && (compareCost = compare1->GetCost() + compare2->GetCost()) < tempCost){
                        next = (* tableData)[j][0].GetNumber();
                        newCost = compareCost;
                        tempCost = newCost;
                    }
                }
                (* dest).SetCost(tempCost);
                (* dest).SetNextHop(next);

            }
        
        }
    }



	

	
    Entry::Entry(){}

    Entry::Entry(unsigned dN, unsigned nH, double c){
    	cost = c;
    	destNode = dN;
    	nextHop = nH;
    }

    Entry::Entry(const Entry (& e)):
    cost(e.cost), destNode(e.destNode), nextHop(e.nextHop)
    {
    	//SetCost(e.GetCost());
    	//destNode = e.GetNumber();
    	//nextHop = e.GetNextHop();
    }

    unsigned Entry::GetNumber() const{
        return destNode;
    }

    unsigned Entry::GetNextHop() const{
        return nextHop;
    }

    double Entry::GetCost() const{
        return cost;
    }

    void Entry::SetCost(double c){
        cost = c;
    }

    void Entry::SetNextHop(unsigned nH){
    	nextHop = nH;
    }

#endif


