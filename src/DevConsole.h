class DevConsole
{
public:
    static DevConsole* Init();
	void logMessage(const char* message);
	void logError(const char* error);	

private:
	DevConsole();
    ~DevConsole();
	static DevConsole* m_Singletone;
    
};
