#ifndef PLAYERS_DATA_H
#define PLAYERS_DATA_H

#define TEAM_COUNT 10
#define PLAYERS_PER_TEAM 20
#define TOTAL_INITIAL_PLAYERS (TEAM_COUNT * PLAYERS_PER_TEAM)

static const char TEAM_NAMES[TEAM_COUNT][50] = {
    "India",
    "Australia",
    "England",
    "Pakistan",
    "New Zealand",
    "South Africa",
    "Bangladesh",
    "Sri Lanka",
    "Afghanistan",
    "West Indies"
};

static const char PLAYER_NAMES[TEAM_COUNT][PLAYERS_PER_TEAM][50] = {
    {"Rohit Sharma", "Virat Kohli", "Shubman Gill", "Shreyas Iyer", "KL Rahul",
     "Suryakumar Yadav", "Rishabh Pant", "Hardik Pandya", "Ravindra Jadeja",
     "Axar Patel", "Jasprit Bumrah", "Mohammed Shami", "Mohammed Siraj",
     "Kuldeep Yadav", "Yuzvendra Chahal", "R Ashwin", "Arshdeep Singh",
     "Washington Sundar", "Tilak Varma", "Sanju Samson"},
    {"David Warner", "Steven Smith", "Travis Head", "Marnus Labuschagne", "Steve Smith",
     "Glenn Maxwell", "Mitchell Marsh", "Marcus Stoinis", "Pat Cummins",
     "Mitchell Starc", "Josh Hazlewood", "Adam Zampa", "Alex Carey",
     "Cameron Green", "Matthew Wade", "Nathan Lyon", "Sean Abbott",
     "Jhye Richardson", "Aaron Hardie", "Josh Inglis"},
    {"Jos Buttler", "Joe Root", "Harry Brook", "Ben Stokes", "Jonny Bairstow",
     "Phil Salt", "Dawid Malan", "Sam Curran", "Liam Livingstone", "Moeen Ali",
     "Mark Wood", "Jofra Archer", "Chris Woakes", "Adil Rashid", "Reece Topley",
     "Gus Atkinson", "Brydon Carse", "David Willey", "Will Jacks", "Tom Hartley"},
    {"Babar Azam", "Mohammad Rizwan", "Fakhar Zaman", "Imam-ul-Haq", "Saud Shakeel",
     "Saim Ayub", "Agha Salman", "Iftikhar Ahmed", "Shadab Khan", "Mohammad Nawaz",
     "Shaheen Afridi", "Haris Rauf", "Naseem Shah", "Hasan Ali", "Abrar Ahmed",
     "Mohammad Amir", "Zaman Khan", "Usama Mir", "Faheem Ashraf", "Kamran Ghulam"},
    {"Devon Conway", "Will Young", "Rachin Ravindra", "Daryl Mitchell", "Tom Latham",
     "Finn Allen", "Glenn Phillips", "Mark Chapman", "Michael Bracewell", "Mitchell Santner",
     "Tim Southee", "Trent Boult", "Matt Henry", "Lockie Ferguson", "Ben Sears",
     "Ish Sodhi", "Kyle Jamieson", "Jacob Duffy", "Phillip Fisk", "James Neesham"},
    {"Temba Bavuma", "Quinton de Kock", "Aiden Markram", "Rassie van der Dussen",
     "David Miller", "Heinrich Klaasen", "Tristan Stubbs", "Marco Jansen",
     "Wiaan Mulder", "Andile Phehlukwayo", "Kagiso Rabada", "Anrich Nortje",
     "Lungi Ngidi", "Keshav Maharaj", "Tabraiz Shamsi", "Gerald Coetzee",
     "Ottniel Baartman", "Nandre Burger", "Bjorn Fortuin", "Ryan Rickelton"},
    {"Litton Das", "Najmul Hossain Shanto", "Tanzid Hasan", "Towhid Hridoy",
     "Mahmudullah", "Mushfiqur Rahim", "Mehidy Hasan Miraz", "Shakib Al Hasan",
     "Soumya Sarkar", "Jaker Ali", "Taskin Ahmed", "Mustafizur Rahman",
     "Tanzim Hasan", "Shoriful Islam", "Hasan Mahmud", "Taijul Islam",
     "Nasum Ahmed", "Nahid Rana", "Rishad Hossain", "Afif Hossain"},
    {"Kusal Mendis", "Pathum Nissanka", "Kusal Perera", "Charith Asalanka",
     "Sadeera Samarawickrama", "Avishka Fernando", "Dasun Shanaka", "Dhananjaya de Silva",
     "Chamika Karunaratne", "Wanindu Hasaranga", "Maheesh Theekshana",
     "Dilshan Madushanka", "Dushmantha Chameera", "Lahiru Kumara",
     "Matheesha Pathirana", "Kasun Rajitha", "Pramod Madushan", "Jeffrey Vandersay",
     "Janith Liyanage", "Nuwan Thushara"},
    {"Rahmanullah Gurbaz", "Ibrahim Zadran", "Hazratullah Zazai", "Najibullah Zadran",
     "Azmatullah Omarzai", "Mohammad Nabi", "Rashid Khan", "Mujeeb Ur Rahman",
     "Noor Ahmad", "Naveen-ul-Haq", "Fazalhaq Farooqi", "Ikram Alikhil",
     "Hashmatullah Shahidi", "Karim Janat", "Gulbadin Naib", "Sharafuddin Ashraf",
     "Fareed Ahmad", "Qais Ahmad", "Darwish Rasooli", "Allah Mohammad"},
    {"Brandon King", "Shai Hope", "Shimron Hetmyer", "Nicholas Pooran", "Alick Athanaze",
     "Shamarh Brooks", "Keacy Carty", "Rovman Powell", "Romario Shepherd",
     "Kyle Mayers", "Alzarri Joseph", "Gudakesh Motie", "Akeal Hosein",
     "Shamar Joseph", "Jayden Seales", "Obed McCoy", "Matthew Forde",
     "Justin Greaves", "Sherfane Rutherford", "Jason Holder"}
};

#endif
