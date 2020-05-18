//
// UNetwork.h
//

#ifndef __H_UNetwork__
#define __H_UNetwork__

class UNetwork
{
public:
	UNetwork() {};
	~UNetwork() {};

	static BOOL GetHostInfo(char * outIP, char * outName = NULL);
	static void DumpSocketError(void);
};

#endif // __H_UNetwork__