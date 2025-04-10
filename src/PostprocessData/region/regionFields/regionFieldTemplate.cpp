
template<typename T> 
pFlow::regionField<T>::regionField(
    const word& name, 
    const regionPoints& rPoints,
    const T defaultVal)
:
    field_(name, "regionFieldValue", rPoints.size(), rPoints.size(), defaultVal),
    regionPoints_(rPoints)
{}