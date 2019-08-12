namespace zbFile
{
	bool Filename::IsValid()const
	{
		return mzFilename.size() > 3 && mzFilename[0] != L'.';
	}
}  
