
#include "TechManager.h"

TechManager& TechManager::instance()
{
    static TechManager it;
    return it;
}


TechManager::TechManager() : 
    m_next_id(1)
{

}


TechManager::const_iterator TechManager::begin() const
{
    return m_tech_map.begin();
}

TechManager::const_iterator TechManager::end() const
{
    return m_tech_map.end();
}

/** Technology Lookup By ID */ 
 /**
   method to get at a TechLevel object given its ID.
   Returns a pointer to the tech with the given ID, or NULL
   if no such tech exists in the manager.  This is the const version.
*/
const TechLevel* TechManager::Lookup(int ID) const
{
    const_iterator itr = m_tech_map.find(ID);
    
    if(itr == end())
    {
        return NULL;
    }
    else
    {
        return (*itr).second;
    }
}
/**
   method to get at a TechLevel object given its ID.
   Returns a pointer to the tech with the given ID, or NULL
   if no such tech exists in the manager
*/
TechLevel* TechManager::Lookup(int ID)
{
    iterator itr = m_tech_map.find(ID);
    
    if(itr == end())
    {
        return NULL;
    }
    else
    {
        return (*itr).second;
    }
}



TechManager::iterator TechManager::begin() 
{
    return m_tech_map.begin();
}

TechManager::iterator TechManager::end() 
{
    return m_tech_map.end();
}



/**
   reads a file containing technology items and populates itself
   with the technologies in the file.  For V0.1, this will simply
   create the hardcoded technology levels described in TechLevel.h
*/
bool TechManager::LoadTechTree(const std::string& TechFileName)
{
    // make hardcoded techlevels as per v0.1 requirements document
    TechLevel* mark2 = new TechLevel(((int)TechLevel::TECH_MARK2), std::string("Mark II"), 250);
    TechLevel* dbase = new TechLevel(((int)TechLevel::TECH_BASE),  std::string("Defense Base"), 350);
    TechLevel* mark3 = new TechLevel(((int)TechLevel::TECH_MARK3), std::string("Mark III"), 750);
    TechLevel* mark4 = new TechLevel(((int)TechLevel::TECH_MARK4), std::string("Mark IV"), 1650);
    
    std::pair<int, TechLevel*> the_pair;
    
    the_pair.first = (int)TechLevel::TECH_MARK2;
    the_pair.second = mark2;
    m_tech_map.insert(the_pair);
    
    the_pair.first = (int)TechLevel::TECH_BASE;
    the_pair.second = dbase;
    m_tech_map.insert(the_pair);
    
    the_pair.first = (int)TechLevel::TECH_MARK3;
    the_pair.second = mark3;
    m_tech_map.insert(the_pair);
    
    the_pair.first = (int)TechLevel::TECH_MARK4;
    the_pair.second = mark4;
    m_tech_map.insert(the_pair);
    
    return true;
}

/**
   Removes all technologies in the TechManager and deallocates them
   Any pointers, iterators, or references to TechLevel objects in 
   this manager will be invalidated
*/
void TechManager::ClearAll()
{
    m_tech_map.clear();
}

