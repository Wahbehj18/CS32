void listAll(const MenuItem* m, string path) // two-parameter overload
{
	const vector<MenuItem*>* a = m->menuItems();

	if (path == "/")
	{
		path = "";
		cout << m->name() << endl;
	}

	if (path != "")
		cout << path + m->name() << endl;

	if (a != nullptr && a->size() != 0)
	{

		path += m->name() + "/";
		for (int i = 0; i < a->size(); i++)
			listAll((*a)[i], path);
	}
}