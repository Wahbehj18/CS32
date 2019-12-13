class Landmark
{
public:
	Landmark(string nm)
	{
		m_name = nm;
	}
	virtual string color() const = 0;
	virtual string icon() const = 0;
	string name() const
	{
		return m_name;
	}
	virtual ~Landmark(){}
private:
	string m_name;
};

class Hotel : public Landmark
{
public:
	Hotel(string nm)
		:Landmark(nm)
	{}
	virtual string icon() const
	{
		return "bed";
	}
	virtual string color() const
	{
		return "yellow";
	}
	virtual ~Hotel()
	{
		cout << "Destroying the hotel " << Landmark::name() << "." << endl;
	}
};

class Restaurant : public Landmark
{
public:
	Restaurant(string nm, int seats)
		:Landmark(nm)
	{
		m_seats = seats;
	}
	virtual string icon() const
	{
		if (m_seats >= 40)
			return "large knife/fork";
		else
			return "small knife/fork";
	}
	virtual string color() const
	{
		return "yellow";
	}

	virtual ~Restaurant()
	{
		cout << "Destroying the restaurant " << Landmark::name() << "." << endl;;
	}
private:
	int m_seats;
};

class Hospital : public Landmark
{
public:
	Hospital(string nm)
		:Landmark(nm)
	{}
	virtual string icon() const
	{
		return "H";
	}
	virtual string color() const
	{
		return "blue";
	}
	virtual ~Hospital()
	{
		cout << "Destroying the hospital " << Landmark::name() << "." << endl;;
	}
};
