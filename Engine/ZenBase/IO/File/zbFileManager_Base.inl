namespace zbFile
{
	bool Filename::IsValid()const
	{
		return mzFilename.Count() > 3 && mzFilename[0] != L'.';
	}
}  
