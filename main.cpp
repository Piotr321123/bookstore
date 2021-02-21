#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

class Book
{
	friend std::ostream& operator<<(std::ostream&, Book&);
	friend std::istream& operator>>(std::istream&, Book&);
public:
	Book(std::string author, std::string title, float price)
	{
		m_Author = author;
		m_Title = title;
		m_Price = price;
	}

	//Default oznacza, że chcemy używać podanego (w tym wypadku bezparametrowy) konstruktora ale również chcemy 
	//aby kompilator wygenerował jego definicję
	Book() = default;

	//Metoda jest typu const. Oznacza to, że metoda nie zmieni stanu obiektu
	void PrintInfo() const
	{
		std::cout << "\nKsiazka: \"" << m_Title << "\", autor: " << m_Author << ", cena: " << m_Price << " $.\n";
	}

	const std::string GetTitle() const
	{
		return m_Title;
	}

private:
	std::string m_Author;
	std::string m_Title;
	float m_Price;

};


std::ostream& operator<<(std::ostream& in, Book& book) {

	in << book.m_Title << ";" << book.m_Author << ";" << book.m_Price << ";";
	return in;
}

std::istream& operator>>(std::istream& out, Book& book) {
	std::getline(out, book.m_Title, ';');
	std::getline(out, book.m_Author, ';');

	std::string price;
	//getline jako drugi parameter przyjmuje std::string
	std::getline(out, price, ';');
	//Konwertujemy std::string na float
	book.m_Price = std::stof(price);
	return out;
}

class BookShop
{
public:
	//Dodaje książkę do sklepu
	void AddBook(Book book)
	{
		m_Books.push_back(book);
	}

	//Wypisuje wszystkie książki na ekran
	void ShowAll()
	{
		for (std::size_t i = 0; i < m_Books.size(); ++i)
		{
			m_Books[i].PrintInfo();
		}
	}

	//Zapisuje wszystkie książki do pliku
	void SaveToFile(std::string fileName)
	{
		std::ofstream toFile(fileName, std::ios_base::app);
		for (std::size_t i = 0; i < m_Books.size(); ++i)
		{
			toFile << m_Books[i] << std::endl;
		}
		toFile.close();
		std::cout << "Dane zapisane do pliku: " << fileName << "\n";
	}

	//Wczytuje książki z pliku
	void ReadFromFile(std::string fileName)
	{
		std::ifstream fromFile(fileName);
		if (fromFile.is_open()) 
		{
			Book book;
			while((fromFile >> book))
			{
				m_Books.push_back(book);
			}
			std::cout << "Dane wczytane z pliku: " << fileName << "\n";
			fromFile.close();
		}
		else
		{
			std::cout << "Nie można otworzyć pliku: " << fileName << "\n";
		}
	}

	//Wypisuje informacje o książce o podanym tytule
	void FindByTitle(std::string title)
	{
		for (std::size_t i = 0; i < m_Books.size(); ++i)
		{
			if (m_Books[i].GetTitle() == title)
			{
				m_Books[i].PrintInfo();
			}
		}
	}

	//Używamy tzw. Range-based loop oraz automatycznej dedukcji typu (wprowadzonych w C++11). Pozwala 
	//to iterować po kontenerze, który udostępnia metody begin() oraz end(). Co ciekawe do 
	//pojedynczych obiektów możemy odwoływać się przy pomocy referencji, referencji do 
	//niezmienialnego obiektu (const auto &), oraz ich kopii. W poniższym przykładzie, ze względu 
	//na to, że nie potrzebujemy kopiować obiektów, oraz że nie będziemy ich zmieniać, skorzystaliśmy 
	//z referencji do niezmienialnego obiektu.
	void ShowAll2()
	{
		for (const auto & book : m_Books)
		{
			book.PrintInfo();
		}
	}

	//Jeśli w tym momencie złapałeś się za głowę i zacząłeś zastanawiać się czy to nadal jest C++
	//nie martw się!:) Jest to nieco wyższy poziom zaawansowania. Wymaga znajomości algorytmów oraz 
	//anonimowych wyrażeń lambda. Nie wchodząc zbyt głęboko w szczegóły (bo nie to jest celem obecnego posta),
	//funkcja copy_if jako pierwszy parametr przyjmuje iterator wskazujący na początek zakresu 
	//(który będziemy chcieli przeanalizować), drugi to koniec tego przedziału.
	//Trzecim argumentem jest iterator, wskazujący gdzie dane mają być skopiowane.
	//(w naszym przypadku jest to standardowy strumień wyjścia std::cout)
	//Czwartym argumentem jest predykat. Może to być wskaźnik na funckję lub obiekt funkcyjny 
	//(w naszym przypadku lambda). Predykat nie powinien modyfikować obiektów, mna których operuje. Dodatkowo 
	//powinien on zwracać wartość bool w styuacji kiedy chcemy aby obiekt został skopiowany.
	//Do tematów lambd wrócę w przyszłości.
	void FindByTitle2(const std::string & title)
	{
		std::copy_if(m_Books.begin(), m_Books.end(), std::ostream_iterator<Book&>(std::cout, "\n"), 
			[title](Book & book) {
			return book.GetTitle() == title;
			});
	}

private:
	std::vector<Book> m_Books;

};


int main()
{
	std::string fileName = "books.txt";
	//Tworzymy książkę
	Book book = Book("Adam Mickiewicz", "Pan Tadeusz", 46.99f);
	Book book2 = Book("Adam Mickiewicz", "Pan Tadeusz", 46.99f);

	BookShop bookShop;
	bookShop.AddBook(book);
	bookShop.AddBook(book2);

	bookShop.FindByTitle2("Pan Tadeusz");
	//bookShop.ShowAll();
	bookShop.SaveToFile(fileName);

	return 0;
}
