#include <iostream>
#include <string>
#include <algorithm>
#include <cassert>

/*
Edward Wang
ezw0077@auburn.edu
project4_Wang_ezw0077.cpp
sources:
https://www.geeksforgeeks.org/linked-list-of-linked-list/: tutorial of how to deal with linked lists in linked lists
https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case: make everything lowercase without usage of arrays/etc
https://www.geeksforgeeks.org/remove-first-node-of-the-linked-list/: how to delete head of linked list
https://stackoverflow.com/questions/40575482/debug-mode-or-release-mode go in and out of debug mode
*/

using namespace std;

// function to convert a string to lowercase
string toLowerCase(const string &str)
{
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// struct to store user's answer to each question
struct userAnswers
{
    int questionNum;
    string answer;
    userAnswers *next;

    // constructor initializes the answer's question number, answer text, and next pointer
    userAnswers(int qNum = 0, string ans = "") : questionNum(qNum), answer(ans), next(nullptr) {}
};

// struct to store each answer choice for multiple-choice questions
struct answerChoices
{
    string questionLetter;
    string choiceText;
    answerChoices *nextChoice;

    // constructor to initialize each answer choice letter and text
    answerChoices(const string &letter, const string &text)
        : questionLetter(letter), choiceText(text), nextChoice(nullptr) {}
};

// struct to store a quiz question and its associated data
struct theQuestion
{
    string questionType;
    string questionText;
    string correctAnswer;
    double pointValue;
    userAnswers *userAnswers;     // linked list of user answers
    answerChoices *answerChoices; // linked list of answer choices for mcq
    theQuestion *next;            // pointer to the next question

    // constructor initializes question type, text, correct answer, and point value
    theQuestion(const string &type, const string &text, const string &answer, double points)
        : questionType(type), questionText(text), correctAnswer(answer), pointValue(points),
          userAnswers(new struct userAnswers()), answerChoices(nullptr), next(nullptr) {}
};

// function to add a multiple-choice question
theQuestion *addMCQ()
{
    string text;
    string correctAnswer;
    double points;

    // prompt user to enter question text
    cout << "\nEnter a question: ";
    cin.ignore();
    getline(cin, text);
    cout << "[At any time, type 'quit()' to exit] \n"
         << endl;

    // create a new mcq question node
    theQuestion *newQuestion = new theQuestion("mcq", text, "", 0);

    // loop to gather multiple answer choices
    for (char letter = 'A'; letter <= 'E'; letter++)
    {
        string choice;
        cout << "Enter choice " << letter << ": ";
        getline(cin, choice);

        // ensure at least one answer choice is provided
        while (letter == 'A')
        {
            if (choice == "quit()")
            {
                cout << "Question must have at least 1 answer." << endl;
                cout << "\nEnter choice " << letter << ": ";
                getline(cin, choice);
            }
            else
            {
                break;
            }
        }

        // break if quit command is entered after the first choice
        if (choice == "quit()")
        {
            break;
        }

        // create new answer choice node and add it to the linked list
        answerChoices *newChoice = new answerChoices(string(1, letter), choice);
        if (newQuestion->answerChoices == nullptr)
        {
            newQuestion->answerChoices = newChoice;
        }
        else
        {
            answerChoices *temp = newQuestion->answerChoices;
            while (temp->nextChoice != nullptr)
            {
                temp = temp->nextChoice;
            }
            temp->nextChoice = newChoice;
        }
    }

    // prompt user to select correct answer and validate it
    cout << "\nSelect correct answer: ";
    while (true)
    {
        cin >> correctAnswer;
        bool found = false;
        answerChoices *temp = newQuestion->answerChoices;

        // traverse answer choices to check if the entered answer exists
        while (temp != nullptr)
        {
            if (toLowerCase(temp->questionLetter) == toLowerCase(correctAnswer))
            {
                found = true;
                break;
            }
            temp = temp->nextChoice;
        }

        if (found)
        {
            newQuestion->correctAnswer = correctAnswer;
            break;
        }
        else
        {
            cout << "[Answer not recognized, please try again!] \n Select correct answer: ";
        }
    }

    // prompt user for the point value of the question
    cout << "Enter the point value: ";
    while (true)
    {
        cin >> points;
        if (cin.fail() || points <= 0)
        {
            cout << "[Not a point value, please try again!] \n Enter the point value: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else
        {
            newQuestion->pointValue = points;
            break;
        }
    }

    return newQuestion;
}

// function to add a true/false question
theQuestion *addTF()
{
    string text;
    string correctAnswer;
    double points;

    cin.ignore();
    cout << "\nEnter a question: ";
    getline(cin, text);
    cout << "\n";

    // prompt user for the correct answer and validate it
    while (true)
    {
        cout << "Select correct answer: ";
        getline(cin, correctAnswer);
        if (toLowerCase(correctAnswer) == "true" || toLowerCase(correctAnswer) == "false")
        {
            break;
        }
        else
        {
            cout << "Invalid input. Please enter 'true' or 'false'. \n";
        }
    }

    // prompt user for the point value
    cout << "Enter the point value: ";
    cin >> points;
    cin.ignore();

    return new theQuestion("tf", text, correctAnswer, points);
}

// function to add a written response question
theQuestion *addWR()
{
    string text;
    string correctAnswer;
    double points;

    cin.ignore();
    cout << "\nEnter a question: ";
    getline(cin, text);
    cout << "\n";

    // prompt user for the correct answer
    cout << "Select correct answer: ";
    getline(cin, correctAnswer);

    // prompt user for the point value
    cout << "Enter the point value: ";
    cin >> points;
    cin.ignore();

    return new theQuestion("wr", text, correctAnswer, points);
}

// function to display all questions in the quiz
void displayQuiz(theQuestion *head)
{
    theQuestion *current = head;

    // iterate through each question and display details
    while (current != nullptr)
    {
        cout << "\nQuestion Type: " << current->questionType << endl;
        cout << "Question: " << current->questionText << endl;
        cout << "Correct Answer: " << current->correctAnswer << endl;
        cout << "Point Value: " << current->pointValue << endl;

        // display answer choices if the question is mcq
        if (current->questionType == "mcq")
        {
            answerChoices *choice = current->answerChoices;
            while (choice != nullptr)
            {
                cout << choice->questionLetter << ": " << choice->choiceText << endl;
                choice = choice->nextChoice;
            }
        }

        current = current->next;
    }
}

void editQuestion(theQuestion *head, int questionNumber)
{
    theQuestion *current = head;
    int currentQuestion = 1;
    string editNumber;
    string newQuestion;
    string newAnswer;
    string newChoiceAnswer;

    // traverse the linked list to find the question to edit
    while (current != nullptr && currentQuestion != questionNumber)
    {
        current = current->next;
        currentQuestion++;
    }

    // if question does not exist, notify user and exit
    if (current == nullptr)
    {
        cout << "That question does not exist!" << endl;
        return;
    }

    // display saved details of the selected question
    cout << "===============================" << endl;
    cout << "=== QUESTION " << questionNumber << " SAVED VALUES ===" << endl;
    cout << "===============================" << endl;

    // if question type is multiple choice, display choices and allow editing
    if (current->questionType == "mcq")
    {
        cout << "  1. Type: " << current->questionType << endl;
        cout << "  2. Question: " << current->questionText << endl;

        answerChoices *choice = current->answerChoices;
        cout << "  3. Answer choices: " << endl;
        while (choice != nullptr)
        {
            cout << "    " << choice->questionLetter << ". " << choice->choiceText << endl;
            choice = choice->nextChoice;
        }

        cout << "  4. Correct answer: " << current->correctAnswer << endl;
        cout << "===============================" << endl;

        // loop to allow editing different parts of the question
        while (true)
        {
            cout << "Type a number to edit, or type quit(): ";
            cin >> editNumber;
            if (editNumber == "quit()")
            {
                break;
            }

            int number;
            try
            {
                number = stoi(editNumber); // convert input to integer
            }
            catch (invalid_argument &)
            {
                cout << "Invalid input, please enter a valid number or type quit()." << endl;
                continue;
            }

            if (number < 1 || number > 4)
            {
                cout << "Invalid number, please enter a number between 1 and 4 or type quit(): " << endl;
                continue;
            }

            switch (number)
            {
            case 1:
                cout << "Can't edit question type!" << endl;
                break;
            case 2:
                // prompt user for new question text
                cout << "Enter new question: " << endl;
                cin.ignore(1000, '\n'); // clear the buffer
                getline(cin, newQuestion);
                current->questionText = newQuestion;
                cout << "Question updated!" << endl;
                break;
            case 3:
                // edit each answer choice, allowing 'quit()' to stop editing choices
                choice = current->answerChoices;
                cin.ignore(1000, '\n');
                while (choice != nullptr)
                {
                    cout << "Enter new choice for " << choice->questionLetter << ": ";
                    getline(cin, newChoiceAnswer);
                    if (newChoiceAnswer == "quit()")
                    {
                        choice->choiceText = "";
                        choice->nextChoice = nullptr;
                        break;
                    }
                    choice->choiceText = newChoiceAnswer;
                    choice = choice->nextChoice;
                }
                // reset choices with empty text to null
                current = head;
                choice = current->answerChoices;
                while (choice != nullptr)
                {
                    if (choice->nextChoice->choiceText == "")
                    {
                        choice->nextChoice = nullptr;
                    }
                    choice = choice->nextChoice;
                }

                cout << "Choices updated!" << endl;
                break;
            case 4:
                // prompt user to enter new correct answer
                cout << "Enter new correct answer: ";
                cin.ignore(1000, '\n');
                getline(cin, newAnswer);
                current->correctAnswer = newAnswer;
                cout << "Correct answer updated!" << endl;
                break;
            }
        }
    }
    else // handling for true/false and written response types
    {
        cout << "1. Type: " << current->questionType << endl;
        cout << "2. Question: " << current->questionText << endl;
        cout << "3. Correct answer: " << current->correctAnswer << endl;
        cout << "===============================" << endl;

        while (true)
        {
            cout << "Type a number to edit, or type quit(): ";
            cin >> editNumber;
            if (editNumber == "quit()")
            {
                break;
            }

            int number;
            try
            {
                number = stoi(editNumber);
            }
            catch (invalid_argument &)
            {
                cout << "Invalid input, please enter a valid number or type quit()." << endl;
                continue;
            }

            if (number < 1 || number > 3)
            {
                cout << "Invalid number, please enter a number between 1 and 3 or type quit(): " << endl;
                continue;
            }

            switch (number)
            {
            case 1:
                cout << "Can't edit question type!" << endl;
                break;
            case 2:
                // edit question text
                cout << "Enter new question: " << endl;
                cin.ignore(1000, '\n');
                getline(cin, newQuestion);
                current->questionText = newQuestion;
                cout << "Question updated!" << endl;
                break;
            case 3:
                // edit correct answer for true/false or written response question
                cout << "Enter new correct answer: " << endl;
                cin.ignore(1000, '\n');
                getline(cin, newAnswer);
                current->correctAnswer = newAnswer;
                cout << "Correct answer updated!" << endl;
                break;
            }
        }
    }
}

// function to delete a specific question from the quiz
bool deleteQuestion(theQuestion *&head, int questionNumber)
{
    if (head == nullptr)
    {
        cout << "The quiz is empty. No question to delete." << endl;
        return false;
    }

    // special case for deleting the first question
    if (questionNumber == 1)
    {
        theQuestion *temp = head;
        head = head->next; // move head to the next question
        delete temp;       // free memory of the deleted question
        cout << "Question " << questionNumber << " deleted." << endl;
        return true;
    }

    // find the question before the one to delete
    theQuestion *current = head;
    int currentPosition = 1;

    while (current != nullptr && currentPosition < questionNumber - 1)
    {
        current = current->next;
        currentPosition++;
    }

    // if we reach the end or question number is invalid
    if (current == nullptr || current->next == nullptr)
    {
        cout << "The question number does not exist." << endl;
        return false;
    }

    // unlink and delete the specified question node
    theQuestion *questionToDelete = current->next;
    current->next = questionToDelete->next; // link to the following question
    delete questionToDelete;                // free memory of the deleted question
    cout << "Question " << questionNumber << " deleted." << endl;
    return true;
}

void assessQuiz(theQuestion *head)
{
    theQuestion *current = head;
    double totalScore = 0.0;
    double totalUserScore = 0.0;
    int numOfQuestion = 0;
    string userAnswer;

    // calculate total score and number of questions
    while (current != nullptr)
    {
        totalScore += current->pointValue;
        current = current->next;
        numOfQuestion++;
    }

    current = head; // reset current pointer to the beginning of the quiz

    bool allAnswered = false;
    bool assessmentComplete = false;
    char submitTest;

    // loop until assessment is submitted or all questions are answered
    while (!assessmentComplete)
    {
        if (allAnswered)
        {
            cout << "All questions answered. ";
        }

        int action;
        cout << "Do you want to?" << endl;
        cout << "1. Go to next question." << endl;
        cout << "2. Jump to question." << endl;
        cout << "3. Submit." << endl;
        cout << "Select an action: ";

        // validate input for action selection
        while (!(cin >> action) || action < 1 || action > 3)
        {
            cout << "Invalid input. Please enter a number between 1 and 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(1000, '\n'); // clear input buffer

        switch (action)
        {
        case 1: // go to the next unanswered question
        {
            bool foundUnanswered = false;
            theQuestion *start = current;

            // search for the next unanswered question
            while (current != nullptr)
            {
                if (!current->userAnswers || current->userAnswers->answer.empty())
                {
                    foundUnanswered = true;
                    cout << "\nQuestion: " << current->questionText << endl;

                    // display answer choices if it's an mcq
                    if (current->questionType == "mcq")
                    {
                        answerChoices *choice = current->answerChoices;
                        while (choice != nullptr)
                        {
                            cout << choice->questionLetter << ": " << choice->choiceText << endl;
                            choice = choice->nextChoice;
                        }
                    }

                    // prompt user for an answer
                    cout << "Your answer: ";
                    getline(cin, userAnswer);

                    if (!current->userAnswers)
                        current->userAnswers = new userAnswers(numOfQuestion, userAnswer);
                    else
                        current->userAnswers->answer = userAnswer;

                    current = current->next;
                    break;
                }
                current = current->next;

                // loop back to the beginning if end is reached
                if (current == nullptr && start != head)
                {
                    current = head;
                }

                if (current == start) // stop if all questions are checked
                {
                    break;
                }
            }

            // if no unanswered question was found, proceed to the next question in usual order
            if (!foundUnanswered)
            {
                allAnswered = true;
                if (current == nullptr)
                    current = head; // start from the beginning if at the end

                if (current != nullptr)
                {
                    cout << "\nQuestion: " << current->questionText << endl;
                    if (current->questionType == "mcq")
                    {
                        answerChoices *choice = current->answerChoices;
                        while (choice != nullptr)
                        {
                            cout << choice->questionLetter << ": " << choice->choiceText << endl;
                            choice = choice->nextChoice;
                        }
                    }

                    cout << "Your answer: ";
                    getline(cin, userAnswer);

                    if (!current->userAnswers)
                        current->userAnswers = new userAnswers(numOfQuestion, userAnswer);
                    else
                        current->userAnswers->answer = userAnswer;

                    current = current->next;
                }
            }
        }
        break;

        case 2: // jump to a specific question
            int questionNum;
            cout << "Jump to question [1-" << numOfQuestion << "]: ";
            while (!(cin >> questionNum) || questionNum < 1 || questionNum > numOfQuestion)
            {
                cout << "Invalid question number. Please enter a valid question number [1-" << numOfQuestion << "]: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore(1000, '\n');

            current = head;
            for (int i = 1; i < questionNum && current != nullptr; i++)
            {
                current = current->next;
            }
            if (current)
            {
                cout << "\nQuestion: " << current->questionText << endl;
                if (current->userAnswers && !current->userAnswers->answer.empty())
                {
                    cout << "You answered: " << current->userAnswers->answer << endl;
                    cout << "Do you want to edit this answer? (y/n): ";
                    char editChoice;
                    cin >> editChoice;
                    cin.ignore(1000, '\n');
                    if (editChoice == 'y' || editChoice == 'Y')
                    {
                        cout << "Your new answer: ";
                        getline(cin, userAnswer);
                        current->userAnswers->answer = userAnswer;
                    }
                }
                else
                {
                    cout << "Your answer: ";
                    getline(cin, userAnswer);

                    if (!current->userAnswers)
                        current->userAnswers = new userAnswers(questionNum, userAnswer);
                    else
                        current->userAnswers->answer = userAnswer;
                }
            }
            break;

        case 3: // submit the test
            allAnswered = true;
            current = head;
            while (current != nullptr)
            {
                if (!current->userAnswers || current->userAnswers->answer.empty())
                {
                    allAnswered = false;
                    break;
                }
                current = current->next;
            }

            if (!allAnswered)
            {
                cout << "WARNING: You still have unanswered question(s) left, are you sure you want to submit? (y/n) ";
                cin >> submitTest;
                cin.ignore(1000, '\n');
                if (submitTest == 'y' || submitTest == 'Y')
                {
                    assessmentComplete = true;
                }
            }
            else
            {
                assessmentComplete = true;
            }
            break;

        default:
            cout << "Invalid action. Please try again.\n";
        }
    }

    // final assessment evaluation after submission
    current = head;
    int questionIndex = 1;
    int correctAnswers = 0;
    cout << "\n/!\\ Assessment Complete.\n=== SESSION LOG ===\n";
    while (current != nullptr)
    {
        cout << "Question " << questionIndex++ << ": " << current->questionText << endl;
        cout << "Your answer: " << current->userAnswers->answer << endl;
        cout << "Correct answer: " << current->correctAnswer << endl;

        // check if user's answer matches the correct answer
        if (toLowerCase(current->userAnswers->answer) == toLowerCase(current->correctAnswer))
        {
            totalUserScore += current->pointValue;
            correctAnswers++;
        }
        current = current->next;
    }

    // display final score and thank user
    cout << "Final Score: " << totalUserScore << "/" << totalScore << "\n"
         << endl;
    cout << "*** Thank you for using the testing service. Goodbye! ***" << endl;
}

// IMPORTANT: REMOVE INDENTS FOR UNIT TESTING!!!!!!!!!!!!!!!!!
//  #define UNIT_TESTING
#ifdef UNIT_TESTING
int main()
{
    cout << "*** This is a debugging version ***" << endl;

    // Unit Test Case 1: Adding a question to an empty quiz
    cout << "Unit Test Case 1: Adding a question to an initially empty quiz.\n";
    theQuestion *quiz1 = addMCQ();
    assert(quiz1 != nullptr); // Ensure quiz is no longer empty
    cout << "Added a question successfully.\n";
    cout << "Case 1 passed.\n"
         << endl;

    // Unit Test Case 2: Checking question and answer storage
    cout << "Unit Test Case 2: Verifying question text and correct answer storage.\n";
    theQuestion *quiz2 = addMCQ();
    quiz2->questionText = "What is 2 + 2?";
    quiz2->correctAnswer = "4";
    assert(quiz2->questionText == "What is 2 + 2?");
    assert(quiz2->correctAnswer == "4");
    cout << "Question text and answer stored correctly.\n";
    cout << "Case 2 passed.\n"
         << endl;

    // Unit Test Case 3: Attempting to delete a question from a quiz with only one question
    cout << "Unit Test Case 3: Deleting the only question in the quiz.\n";
    theQuestion *quiz3 = addMCQ();
    deleteQuestion(quiz3, 1);
    assert(quiz3 == nullptr);
    cout << "The only question deleted. Quiz is now empty.\n";
    cout << "Case 3 passed.\n"
         << endl;

    // Unit Test Case 4: Adding multiple questions and verifying linked structure
    cout << "Unit Test Case 4: Adding multiple questions and verifying linked list structure.\n";
    theQuestion *quiz4 = addMCQ();
    quiz4->next = addTF();
    theQuestion *secondQuestion = quiz4->next;
    assert(quiz4->next == secondQuestion);
    cout << "Questions linked correctly in the quiz.\n";
    cout << "Case 4 passed.\n"
         << endl;

    // Unit Test Case 5: Simulating a complete quiz assessment without answering
    cout << "Unit Test Case 5: Attempting to submit the quiz without user answer.\n";
    theQuestion *quiz5 = addMCQ();

    // simulate empty quiz submission and check that no points are awarded
    double initialScore = 0.0;
    assessQuiz(quiz5);
    assert(quiz5->userAnswers->answer.empty());

    cout << "Quiz submitted without answering question.\n";
    cout << "Case 5 passed.\n"
         << endl;

    cout << "*** End of Debugging Version ***" << endl;
}
#else

int main()
{
    theQuestion *quiz = nullptr; // start with an empty quiz
    theQuestion *tail = nullptr; // tail pointer to keep track of the end of the list
    int choice = 0;
    int numOfQuestion = 0;
    string questionType;
    bool decision = false;

    // welcome message
    cout << "*** Welcome to Wang's Testing Service *** \n"
         << endl;

    // main loop for handling user actions
    do
    {
        theQuestion *newQuestion = nullptr; // placeholder for new question node
        cout << "Do you want to?" << endl;
        cout << "   1. Create new question." << endl;
        cout << "   2. Edit question." << endl;
        cout << "   3. Delete question." << endl;
        cout << "   4. Finish." << endl;
        cout << "Select an action: ";
        cin >> choice;

        // validate user input for choice selection
        while (cin.fail() || (choice < 1 || choice > 4))
        {
            cout << "Invalid option, please enter a number between 1 and 4: ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> choice;
        }
        cout << "\n";

        // handle each choice
        switch (choice)
        {
        case 1: // create new question
            numOfQuestion++;
            cout << "== QUESTION " << numOfQuestion << " ===" << endl;
            cout << "Type of question [mcq/tf/wr]: ";
            cin >> questionType;
            while ((questionType != "mcq") && (questionType != "tf") && (questionType != "wr"))
            {
                cout << "[Command not recognized, please try again!]" << endl;
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\n";
                cout << "Type of question [mcq/tf/wr]: ";
                cin >> questionType;
            }

            // create the question based on type
            if (questionType == "mcq")
            {
                newQuestion = addMCQ();
            }
            else if (questionType == "tf")
            {
                newQuestion = addTF();
            }
            else if (questionType == "wr")
            {
                newQuestion = addWR();
            }

            // add the new question to the quiz list
            if (quiz == nullptr)
            {
                quiz = newQuestion;
                tail = newQuestion;
            }
            else
            {
                tail->next = newQuestion;
                tail = newQuestion;
            }
            cout << "Question saved." << endl;
            cout << "\n";
            break;

        case 2: // edit existing question
            if (numOfQuestion < 1)
            {
                cout << "There are no questions to edit!" << endl;
            }
            else
            {
                int questionNumber;
                if (numOfQuestion == 1)
                {
                    cout << "Select a question to edit, or type quit() [1] \n";
                }
                else
                {
                    cout << "Select a question to edit, or type quit() [1-" << numOfQuestion << "] \n";
                }
                cin >> questionNumber;
                editQuestion(quiz, questionNumber); // call edit function
            }
            break;

        case 3: // delete a question
            if (quiz == nullptr)
            {
                cout << "There are no questions to delete!" << endl;
            }
            else
            {
                int questionNumber;
                if (numOfQuestion == 1)
                {
                    cout << "Select a question to delete, or type quit() [1] \n";
                }
                else
                {
                    cout << "Select a question to delete, or type quit() [1-" << numOfQuestion << "] \n";
                }
                cin >> questionNumber;
                if (deleteQuestion(quiz, questionNumber)) // decrement count if delete successful
                {
                    numOfQuestion--;
                }
            }
            break;

        case 4: // finish and optionally assess the quiz
            if (quiz == nullptr)
            {
                cout << "This quiz is empty!" << endl;
            }
            else
            {
                int questionCount = 0;
                double totalPts = 0.0;
                theQuestion *currentQuestion = quiz;

                // count questions and calculate total points in the quiz
                while (currentQuestion != nullptr)
                {
                    questionCount++;
                    totalPts += currentQuestion->pointValue;
                    currentQuestion = currentQuestion->next; // move to the next question
                }

                // display session log with question count and total points
                cout << "=== SESSION LOG ===" << endl;
                cout << "Total questions: " << questionCount << endl;
                cout << "Total point values: " << totalPts << endl;
                cout << "\n";
                cout << "/!\\ Begin assessment? [y/n]: ";
                char startAssessment;
                cin >> startAssessment;
                cin.ignore(1000, '\n'); // clear input buffer

                // if user chooses yes, proceed with quiz assessment
                if (startAssessment == 'y' || startAssessment == 'Y')
                {
                    decision = true;
                    assessQuiz(quiz); // call assessQuiz function to conduct quiz
                }
            }
            cout << "\n";
            break;
        }
    } while (!decision); // loop until user decides to finish and possibly assess quiz
}
#endif
