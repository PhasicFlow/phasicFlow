
template<typename T>
bool pFlow::MPI::dataIOMPI<T>::gatherData(span<T> data )
{
	
	if(this->ioPattern_.isAllProcessorsDifferent())
	{
		this->bufferSpan_ = data;
		return true;
	}

	if( this->ioPattern_.isMasterProcessorDistribute())
	{
		
		auto gatherT = pFlow::MPI::gatherMaster<T>(pFlowProcessors());
		
		if(!gatherT.gatherData(data))
		{
			fatalErrorInFunction<<"Error in gathering data to master"<<endl;
			return false;
		}

		this->buffer_ = gatherT.moveData();
		this->bufferSpan_ = span<T>(this->buffer_.data(),this->buffer_.size() );

		return true;
	
	}

	if( this->ioPattern_.isMasterProcessorOnly() || this->ioPattern_.isAllProcessorSimilar() )
	{
		if( this->ioPattern_.isMaster() )
		{
			this->bufferSpan_ = data;
			return true;
		}
		else
		{
			this->bufferSpan_ = span<T>(nullptr, 0);
			return true;
		}
	}

	return false;
	
}

template<typename T>
pFlow::MPI::dataIOMPI<T>::dataIOMPI(const IOPattern& iop)
:
	dataIO<T>(iop)
{}