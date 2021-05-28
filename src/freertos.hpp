typedef bool (*Take)(void *semaphore, unsigned int blockTime);
bool take(void *semaphore, unsigned int blockTime);

typedef bool (*Give)(void *semaphore);
bool give(void *semaphore);

typedef void (*Log)(const char *message);
void log(const char *message);