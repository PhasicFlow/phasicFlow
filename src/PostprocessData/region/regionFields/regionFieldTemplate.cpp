


namespace pFlow::postprocessData
{

template<typename T> 
regionField<T>::regionField(
    const word& name, 
    const regionPoints& rPoints,
    const T defaultVal)
:
    field_(name, "regionFieldValue", rPoints.size(), rPoints.size(), defaultVal),
    regionPoints_(rPoints)

{}

template<typename T>
inline
bool regionField<T>::writeFieldToVtk(iOstream& os)const
{
    fatalErrorInFunction<< "This type is not supported for vtk conversion:"<<
	field_.typeName()<<endl;
	fatalExit;
    return false;
}

template<>
inline
bool regionField<real>::writeFieldToVtk(iOstream& os)const
{
    os<<"FIELD FieldData 1 " << field_.name() << " 1 "<< field_.size() << " float\n";
    
    
    const auto mapper = cellMapper{shape()};

    for(uint32 k=0; k<mapper.nz(); k++)
    {
        for(uint32 j=0; j<mapper.ny(); j++)
        {
            for(uint32 i=0; i<mapper.nx(); i++)
            {
                os<< field_[ mapper(i,j,k) ]<<'\n';
            }
        }
    }
    os<<endl;
    return true;
}

template<>
inline
bool regionField<realx3>::writeFieldToVtk(iOstream& os)const
{
    os<<"FIELD FieldData 1 " << field_.name() << " 3 "<< field_.size() << " float\n";
    
    
    const auto mapper = cellMapper{shape()}; 

    for(uint32 k=0; k<mapper.nz(); k++)
    {
        for(uint32 j=0; j<mapper.ny(); j++)
        {
            for(uint32 i=0; i<mapper.nx(); i++)
            {
                os<<field_[mapper(i,j,k)].x()<<' '<<field_[mapper(i,j,k)].y()<<' '<<field_[mapper(i,j,k)].z()<<'\n';
            }
        }
    }
    os<<endl;
    return true;
}

template<>
inline
bool regionField<uint32>::writeFieldToVtk(iOstream& os)const
{
    os<<"FIELD FieldData 1 " << field_.name() << " 1 "<< field_.size() << " int\n";
    
    
    const auto mapper = cellMapper{shape()} ; 

    for(uint32 k=0; k<mapper.nz(); k++)
    {
        for(uint32 j=0; j<mapper.ny(); j++)
        {
            for(uint32 i=0; i<mapper.nx(); i++)
            {
                os<< field_[ mapper(i,j,k) ]<<'\n';
            }
        }
    }
    os<<endl;
    return true;
}

} // End namespace pFlow::postprocessData