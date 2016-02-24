// INCLUDES:

// STRUCTS:

// DEFINES:

// CLASS:
class Fixed1WayQueue
{
	public:
		Fixed1WayQueue(int size);
		void push(int value);
		int get(int index);

	private:
		int currentsize;
		int maxsize;
		int * queue;

}