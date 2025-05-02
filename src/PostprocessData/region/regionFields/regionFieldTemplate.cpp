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

} // End namespace pFlow::postprocessData