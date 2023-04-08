// HLP2: question1.cpp
// 03/28/2022
// A quiz consists of questions, and there are different kinds of questions:
// fill in the blank; choice [single or multiple]; numeric [where approximate
// answer is ok, e.g., 1.33 is ok for 4/3]; free response; and so on.
// An inheritance hierarchy for these question types would consist of
// a root of type Question [that incorporates everything common to all questions
// such as displaying its text and checking for the correct response].
// The other question types can be derived from type Question.

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// base class
class Question {
public:
  Question() {}
  void set_text(std::string const& question_text) { text = question_text; }
  void set_answer(std::string const& correct_response) { answer = correct_response; }
  bool check_answer(std::string const& response) const { return response == answer; }
  void display() const { std::cout << text << std::endl; }
private:
  std::string text;
  std::string answer;
};

/*
In C++, you form a derived class from a base class by specifying what makes the
derived class different. You define the member functions that are new to the derived
class. The derived class inherits all member functions from the base class, but you can
change the implementation if the inherited behavior is not appropriate.
The derived class automatically inherits all data members from the base class. You
only define those data members unique to the derived class.
In short, a derived class inherits data and behavior from its base class.
We want an "is-a" relationship between derived and base classes.
That is, anywhere we can use a Question type we should be able to use derived class type.

To get an "is-a" relationship, we need the derivation list to be public: the public
members of the base class become part of the interface of the derived class as well.

A ChoiceQuestion object differs from a Question object in three ways:
a) Its objects store the various choices for the answer: std::vector<std::string> choices
b) There is a member function for adding another choice: add_choice()
c) The display function of the ChoiceQuestion class shows these choices
so that the respondent can choose one of them: override base class display()

Final things to remember:
0) The keyword public in the derivation list means that derived class wants to
present public interface of base class as part of its public interface!!!
1) A derived class can override a base class function by 
providing a new implementation.
2) The derived class inherits all data members and all functions
that it does not override.
3) The private data of base class will always be private to the base class!!!
*/
class ChoiceQuestion : public Question {
public:
  ChoiceQuestion() {}
  void add_choice(std::string const& choice, bool correct) {
    choices.push_back(choice);
    if (correct) {
      std::string choice_str = std::to_string(choices.size());
      set_answer(choice_str);
    }
  }
  void display() const {
    // private data of base class will always ever be private and cannot
    // be accessed by derived class objects ...
    //std::cout << text << std::endl;
    // syntax for calling base class member function display!!!
    // display(); // will recursively call derived class display()!!!
    Question::display();
    //display();
    std::vector<std::string>::size_type i{};
    for (std::string const& x : choices) {
      std::cout << ++i << ": " << x << std::endl;
    }
  }
private:
// gets text and answer data members from base class Question ...
  std::vector<std::string> choices;
};

int main() {
  // set a basic question ...
  Question q1;
  q1.set_text("Who was the inventor of C++?");
  q1.set_answer("Bjarne Stroustrup");

  // set a choice question ...
  ChoiceQuestion q2;
  q2.set_text("In which country was the inventor of C++ born?");
  q2.add_choice("Singapore", false); q2.add_choice("USA", false);
  q2.add_choice("China", false);     q2.add_choice("India", false);
  q2.add_choice("Sweden", false);    q2.add_choice("Denmark", true);

  // ask a basic question ...
  q1.display(); // Question::display()
  std::cout << "Your answer: ";
  std::string response;
  getline(std::cin, response);
  std::cout << std::boolalpha << q1.check_answer(response) << "\n";

  // ask a choice question ...
  q2.display(); // ChoiceQuestion::display() [which in turn will call Question::display()]
  std::cout << "Your answer: ";
  getline(std::cin, response);
  std::cout << std::boolalpha << q2.check_answer(response) << "\n";
}
