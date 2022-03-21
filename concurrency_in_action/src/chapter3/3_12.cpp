#include<mutex>

struct data_packet {};
struct connection_handle {
	void send_data(data_packet dp) {}
	data_packet receive_data() { return {}; }
};
struct connection_info {};
struct connection_manager {
	static connection_handle open(connection_info cs)
	{
		return {};
	}
};

class X_3_13 {
private:
	connection_info connection_details;
	connection_handle connection;
	std::once_flag connect_init_flag;
	void open_connection()
	{
		connection = connection_manager::open(connection_details);
	}
public:
	X_3_13(connection_info const& connection_details_) : connection_details(connection_details_)
	{}

	void send_data(data_packet const& data)
	{
		// very similar to std::bind
		std::call_once(connect_init_flag, &X_3_13::open_connection, this);
		connection.send_data(data);
	}

	data_packet receive_data()
	{
		// very similar to std::bind
		std::call_once(connect_init_flag, &X_3_13::open_connection, this);
		return connection.receive_data();
	}
};