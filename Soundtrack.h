#ifndef SOUNDTRACK__
#define SOUNDTRACK__

// Some standard headers
#include <string>
#include <cstdlib>

// Class soundtrack
class soundtrack
{
	std::string composer;
	std::string title;
	std::string label;
	std::string catalog_number;
	std::string year_recorded;
	std::string year_released;
	int year_released_data;

public : 

	// Default constructor
	soundtrack();

	// Copy-Constructor
	soundtrack(const soundtrack &rhs);

	// Copy-Assignment
	soundtrack& operator = (const soundtrack &rhs);

	// Check if the soundtrack object is empty
	bool empty() const;

	// Check if the soundtrack is equal to another soundtrack
	// It does not require the program implementation to compare all the values.
	bool operator == (const soundtrack &rhs) const;

	// getComposer() function
	std::string getComposer() const;

	// getTitle() function
	std::string getTitle() const;

	// getLabel() function
	std::string getLabel() const;

	// getCatalogNumber() function
	std::string getCatalogNumber() const;

	// getYearRecorded() function
	std::string getYearRecorded() const;

	// getYearReleased() function
	int getYearReleased() const;

	// getYearReleased() function
	std::string getYearReleasedString() const;

	// setComposer() function
	void setComposer(const std::string &new_composer);

	// setTitle() function
	void setTitle(const std::string &new_title);

	// setLabel() function
	void setLabel(const std::string &new_label);

	// setCatalogNumber() function
	void setCatalogNumber(const std::string &new_catalog_number);

	// setYearRecorded() function
	void setYearRecorded(const std::string &new_year_recorded);

	// setYearReleased() function
	void setYearReleased(const std::string &new_year_released);

	// setYearRecorded() function
	void setYearReleasedString(const std::string &new_year_recorded);

	// setYearReleased() function
	void setYearReleasedNumber(int _year_released);

	// setYearReleased() function
	void setYearReleased(int _year_released);

	// extractComponent() function
	static void extractComponent(const std::string data, std::string &value, int pos, int length);

	bool operator > (const soundtrack &rhs) const;
	bool operator < (const soundtrack &rhs) const;

}; // End class soundtrack

// Overloaded operator function for std::istream (soundtrack)
std::istream &operator >> (std::istream &is, soundtrack &obj);

// Overloaded operator function for std::ostream (soundtrack)
std::ostream &operator << (std::ostream &os, const soundtrack &obj);

#endif