#include "SoundTrack.h"

	// Default constructor
	soundtrack::soundtrack() {year_released_data = 0;}

	// Copy-Constructor
	soundtrack::soundtrack(const soundtrack &rhs) 
	{
		this->setComposer(rhs.getComposer());
		this->setTitle(rhs.getTitle());
		this->setLabel(rhs.getLabel());
		this->setCatalogNumber(rhs.getCatalogNumber());
		this->setYearRecorded(rhs.getYearRecorded());
		this->setYearReleasedString(rhs.getYearReleasedString());	

	} // End constructor (soundtrack(const soundtrack &rhs))

	// Copy-Assignment
	soundtrack& soundtrack::operator = (const soundtrack &rhs) 
	{
		this->setComposer(rhs.getComposer());
		this->setTitle(rhs.getTitle());
		this->setLabel(rhs.getLabel());
		this->setCatalogNumber(rhs.getCatalogNumber());
		this->setYearRecorded(rhs.getYearRecorded());
		this->setYearReleasedString(rhs.getYearReleasedString());	

		return (*this);

	} // End Copy-Assignment (operator = (const soundtrack &rhs))

	// Check if the soundtrack object is empty
	bool soundtrack::empty() const 
	{
		if(this->getComposer().size() > 0) return false;
		if(this->getTitle().size() > 0) return false;
		if(this->getLabel().size() > 0) return false;
		if(this->getCatalogNumber().size() > 0) return false;
		if(this->getYearRecorded().size() > 0) return false;
		if(this->getYearReleasedString().size() > 0) return false;

		return true;
	}

	bool soundtrack::operator > (const soundtrack &rhs) const
	{
		return (this->getCatalogNumber() > rhs.getCatalogNumber());	
	}

	bool soundtrack::operator < (const soundtrack &rhs) const
	{
		return (this->getCatalogNumber() < rhs.getCatalogNumber());		
	}

	// Check if the soundtrack is equal to another soundtrack
	// It does not require the program implementation to compare all the values.
	bool soundtrack::operator == (const soundtrack &rhs) const 
	{
		// However, the comparing soundtrack must not be empty
		if(rhs.empty()) return false;

		if(rhs.getComposer().size() != 0)
		if(this->getComposer() != rhs.getComposer()) return false;
		
		if(rhs.getTitle().size() != 0)
		if(this->getTitle() != rhs.getTitle()) return false;

		if(rhs.getLabel().size() != 0)
		if(this->getLabel() != rhs.getLabel()) return false;

		if(rhs.getCatalogNumber().size() != 0)
		if(this->getCatalogNumber() != rhs.getCatalogNumber()) return false;

		if(rhs.getYearRecorded().size() != 0)
		if(this->getYearRecorded() != rhs.getYearRecorded()) return false;

		if(rhs.getYearReleasedString().size() != 0)
		if(this->getYearReleased() != rhs.getYearReleased()) return false;
		
	
		return true;

	} // End function (operator == (const soundtrack &rhs))

	// getComposer() function
	std::string soundtrack::getComposer() const {return this->composer;}

	// getTitle() function
	std::string soundtrack::getTitle() const {return this->title;}

	// getLabel() function
	std::string soundtrack::getLabel() const {return this->label;}

	// getCatalogNumber() function
	std::string soundtrack::getCatalogNumber() const {return this->catalog_number;}

	// getYearRecorded() function
	std::string soundtrack::getYearRecorded() const {return this->year_recorded;}

	// getYearReleased() function
	int soundtrack::getYearReleased() const {return this->year_released_data;}

	// getYearReleased() function
	std::string soundtrack::getYearReleasedString() const {return this->year_released;}

	// setComposer() function
	void soundtrack::setComposer(const std::string &new_composer){
		this->composer = new_composer;
	}

	// setTitle() function
	void soundtrack::setTitle(const std::string &new_title){
		this->title = new_title;
	}

	// setLabel() function
	void soundtrack::setLabel(const std::string &new_label){
		this->label = new_label;
	}

	// setCatalogNumber() function
	void soundtrack::setCatalogNumber(const std::string &new_catalog_number){
		this->catalog_number = new_catalog_number;
	}

	// setYearRecorded() function
	void soundtrack::setYearRecorded(const std::string &new_year_recorded){
		this->year_recorded = new_year_recorded;
	}

	// setYearReleased() function
	void soundtrack::setYearReleased(const std::string &new_year_released){
		this->setYearReleasedString(new_year_released);
	}

	// setYearReleased() function
	void soundtrack::setYearReleased(int _year_released){
		this->setYearReleasedNumber(_year_released);
	}

	// setYearReleased() function
	void soundtrack::setYearReleasedString(const std::string &new_year_released){
		this->year_released = new_year_released;
		this->year_released_data = atoi(new_year_released.c_str());
	}

	// setYearReleased() function
	void soundtrack::setYearReleasedNumber(int _year_released){
		char year_buffer[80];
		_itoa_s(_year_released, year_buffer, sizeof(year_buffer), 10);

		this->year_released = year_buffer;
		this->year_released_data = _year_released;
	}

void soundtrack::extractComponent(const std::string data, std::string &value, int pos, int length)
{
	int i;
	std::string sub_data = data.substr(pos, length);

	for(i = sub_data.size() - 1; i >= 0; i--)
	{
		if(sub_data[i] != ' ') { i++; break; }
	}

	value = sub_data.substr(0, i);
}

// Overloaded operator function for std::istream (soundtrack)
std::istream &operator >> (std::istream &is, soundtrack &obj)
{
	// Eliminate all the trailing whitespace characters
	is >> std::ws;

	std::string soundtrack_data;

	std::string composer;
	std::string title;
	std::string label;
	std::string catalog_number;
	std::string year_recorded;
	std::string year_released;

	const int composer_length = 24;
	const int title_length = 40;
	const int label_length = 16;
	const int catalog_number_length = 24;
	const int year_recorded_length = 8;
	const int year_released_length = 4;

	const int composer_pos = 0;
	const int title_pos = 0 + composer_length;
	const int label_pos = 0 + composer_length + title_length;
	const int catalog_number_pos = 0 + composer_length + title_length + label_length;
	const int year_recorded_pos = 0 + composer_length + title_length + label_length + catalog_number_length;
	const int year_released_pos = 0 + composer_length + title_length + label_length + catalog_number_length + year_recorded_length;

	// Attempt to read data. If any of these fails, force the function to return
	if(!std::getline(is, soundtrack_data)) return is;

	soundtrack::extractComponent(soundtrack_data, composer, composer_pos, composer_length);
	soundtrack::extractComponent(soundtrack_data, title, title_pos, title_length);
	soundtrack::extractComponent(soundtrack_data, label, label_pos, label_length);
	soundtrack::extractComponent(soundtrack_data, catalog_number, catalog_number_pos, catalog_number_length);
	soundtrack::extractComponent(soundtrack_data, year_recorded, year_recorded_pos, year_recorded_length);
	soundtrack::extractComponent(soundtrack_data, year_released, year_released_pos, year_released_length);

	obj.setComposer(composer);
	obj.setTitle(title);
	obj.setLabel(label);
	obj.setCatalogNumber(catalog_number);
	obj.setYearRecorded(year_recorded);
	obj.setYearReleased(year_released);

	return is;	

} // End function (std::istream &operator >> (std::istream &is, soundtrack &obj))

// Overloaded operator function for std::ostream (soundtrack)
std::ostream &operator << (std::ostream &os, const soundtrack &obj)
{
	os << obj.getComposer() << "  ";
	os << obj.getTitle() << "  ";
	os << obj.getLabel() << "  ";
	os << obj.getCatalogNumber() << "  ";
	os << obj.getYearRecorded() << "  ";
	os << obj.getYearReleasedString();

	return os;	

} // End function (std::istream &operator >> (std::ostream &operator << (std::ostream &os, const soundtrack &obj))