#include <string>
#include <vector>

using cHandle = unsigned int;

class Browser
{
private:
	enum class eConnectionState
	{
		Downloading,	// URL is being downlaoded
		Error,			// Error occured during downlaod, E_OUTOFMEMORY / INET_E_DOWNLOAD_FAILURE
		Finished,		// Download was done sucessfully
		NoConnection,	// No connection was established
		Queued,			// Donwload is in queue
	};

	struct Request
	{
		cHandle m_nHandle;
		std::string m_URL;
		eConnectionState m_eState;
	};

	static inline cHandle m_nReqCount = 0;
	static inline std::vector<Request> m_nRequests;

public:
	Browser() =  delete;
	Browser(Browser&&) = delete;

	// Opens the specified link in default browser
	static void OpenLink(const char *link);

	// Adds a download request 
	static cHandle AddDownloadRequest(const char* link);

	// Returns the current state of the download request
    static eConnectionState GetDownloadRequestState(cHandle handle);

	// Handles the download requests ie actually downloads it
	// Should run in a sperate thread
	static void HandleDownloadRequests();
};