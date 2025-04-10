#include "centerPointsRegionPoints.hpp"
#include "fieldsDataBase.hpp"
#include "Set.hpp"
#include "pStructSelector.hpp"

bool pFlow::centerPointsRegionPoints::selectIds()
{
    if(!firstTimeUpdate_) return true;
    firstTimeUpdate_ = false;
    
    word selector =  probDict_.getVal<word>("selector");
    
    if(selector == "id")
    {
        auto idList = probDict_.getVal<uint32List>("ids");
        Set<uint32> uniqueIds;

        uniqueIds.insert(idList.begin(), idList.end());

        for(auto& id:uniqueIds)
        {
            ids_.push_back(id);
        }
    }
    else
    {
        auto selectorPtr = pStructSelector::create(
            selector, 
            database().pStruct(), 
            probDict_);
        auto selectedPoints = selectorPtr->selectedPoints();
        ids_.resize(selectedPoints.size());
        ids_.assign(selectedPoints.begin(), selectedPoints.end());
    }

    volume_.resize(ids_.size(),1.0);
    diameter_.resize(ids_.size(),1.0);
    center_.resize(ids_.size(), realx3(0,0,0));
    selectedPoints_.resize(ids_.size(), -1);
    return true;
}

pFlow::centerPointsRegionPoints::centerPointsRegionPoints(
    const dictionary &dict,
    fieldsDataBase &fieldsDataBase)
    : regionPoints(dict, fieldsDataBase),
      idName_(dict.getValOrSet<word>("idName", "id")),
      probDict_(dict)
{}

bool pFlow::centerPointsRegionPoints::update()
{
    if(!selectIds()) return false;
    
    if(ids_.empty()) return true;

    const auto& idField = database().updateFieldUint32(idName_);
    selectedPoints_.fill(-1);

    for(uint32 i = 0; i < idField.size(); ++i)
    {
        for( uint32 j=0; j< ids_.size(); ++j)
        {
            if(idField[i] == ids_[j])
            {
                selectedPoints_[j] = i;
                break;
            }
        }
    }

    return true;
}

bool pFlow::centerPointsRegionPoints::write(iOstream &os) const
{
    if(firstTimeUpdate_)
    {
        fatalErrorInFunction
            <<"Write operation should be done before executing the update. \n";
        return false;   
    }
    os <<"# Probing particles for selected ids\n"; 
    
    os << "# SelectedId: ";

    for(auto& id: ids_)
    {
        os << id << tab;
    }
    os << endl;
    os << "time"<<endl;

    return true;
}
