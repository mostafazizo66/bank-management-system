#include "system.hpp"

System::System()
    : window(setResolution(), "Bank System", sf::Style::Titlebar | sf::Style::Close)
{
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(30);
    load(accounts);
    scale = window.getSize().y / 9;
}

sf::VideoMode System::setResolution(){
    sf::RenderWindow preLaunch(sf::VideoMode(800, 450), "Set Resolution", sf::Style::Default);
    preLaunch.setFramerateLimit(10);

    sf::RectangleShape backGround;
    sf::Texture BG;
    BG.loadFromFile("src/Textures/resolutions.jpg");
    setTexture(backGround, BG, sf::Vector2f(0, 0), sf::Vector2f(800, 450));

    sf::VideoMode resolution;
    sf::VideoMode maxResolution = sf::VideoMode::getDesktopMode();
    while(preLaunch.isOpen()){
        sf::Event event;

        while (preLaunch.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                preLaunch.close();
                return sf::VideoMode(1, 1);
            }
            if (event.type == sf::Event::TextEntered) {
                if(event.text.unicode > '0' && event.text.unicode < '7'){
                    switch (event.text.unicode)
                    {
                    case '1': resolution = (sf::VideoMode(800 , 450 )); break;
                    case '2': resolution = (sf::VideoMode(960 , 540 )); break;
                    case '3': resolution = (sf::VideoMode(1280, 720 )); break;
                    case '4': resolution = (sf::VideoMode(1920, 1080)); break;
                    case '5': resolution = (sf::VideoMode(2560, 1440)); break;
                    case '6': resolution = (sf::VideoMode(3840, 2160)); break;
                    }
                    if(resolution.width <= maxResolution.width && resolution.height <= maxResolution.height){
                        return resolution;
                    }
                }
            }
        }
        preLaunch.draw(backGround);
        preLaunch.display();
    }
    return resolution;
}

std::string System::renderLogin(){

    enum Domains{
        username,
        password
    };

    enum Buttons{
        Login,
        Quit
    };

    enum HideState{
        hide,
        view
    };

    // Texts
    sf::Text texts[5];
    std::string strings[] = {"username: ", "password: ", "Email wasn't found", "Wrong password", "Fields can't be empty" , "Login", "Quit"};

    // username & pass
    std::vector<InputDomain> domains;
    for(int i = 0; i < 2; i++){
        domains.push_back(InputDomain(strings[i], sf::Vector2f(scale * 2, scale * 3.25 + scale * i * 1.5),
        scale, sf::Color::White, sf::Color::Black));
    }

    // hide texture
    std::vector<sf::RectangleShape> hideViewButton(2);
    std::vector<sf::Texture> hideViewTextures(2);
    hideViewTextures[hide].loadFromFile("src/Textures/hide.png");
    hideViewTextures[view].loadFromFile("src/Textures/view.png");

    for(int i = hide; i <= view; i++){
        setTexture(hideViewButton[i], hideViewTextures[i], sf::Vector2f(scale * 12.3, scale * 4.55), sf::Vector2f(scale * 1.2, scale * 1.2));
    }
    hideViewButton[view].setPosition(sf::Vector2f(scale * 12.3, scale * 4.5));

    domains[password].hide = true;


    // Error text
    setText(texts[2], sf::Vector2f(scale * 5, scale * 6), sf::Color::Red, scale / 2);

    // Login & Quit menuButtons
    std::vector<Button> menuButtons;
    for(int i = 0; i < 2; i++){
        menuButtons.push_back(Button(strings[i + 5], sf::Vector2f(scale * 5 + scale * 5 * i, scale * 4 + scale * 3), scale * 1.8));
        menuButtons[i].shape.setSize(sf::Vector2f(scale * 3, scale * 1.25));
    }

    int activeBar = -1, errorStat = -1;
    sf::Clock blank;


    /////////////////////////////////////////////////////////////////////
    //////////////////////////// render loop ////////////////////////////
    /////////////////////////////////////////////////////////////////////

    while(window.isOpen()){
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        sf::Event event;

        bool mouseClicked = (event.type == sf::Event::MouseButtonPressed)
         && (event.mouseButton.button == sf::Mouse::Left);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return "";
            }
            if(mouseClicked){
                if(menuButtons[Quit].isTouchingMouse(pos)){
                    window.close();
                    return "";
                }
                for(int i = 0; i < 2; i++){
                    if(hideViewButton[i].getGlobalBounds().contains(pos.x, pos.y)){
                        domains[password].hide = !domains[password].hide;
                        domains[password].flipHiddenText();
                        break;
                    }
                }
                activeBar = -1;
                for(int i = 0; i < 2; i++){
                    if(domains[i].isTouchingMouse(pos)){
                        activeBar = i;
                    }
                }
                if(menuButtons[0].isTouchingMouse(pos)){
                    errorStat = -1;
                    for(int i = 0; i < 2; i++){
                        if(domains[i].input.empty()){
                            errorStat = 3;
                            break;
                        }
                    }
                    if(errorStat != 3){
                        errorStat = login(domains[username].input, domains[password].input);
                    }
                    if(!errorStat){
                        return domains[username].input;
                    }
                }
            }
            if (event.type == sf::Event::TextEntered && activeBar != -1 && domains[activeBar].input.size() <= 30) {

                char enteredChar = static_cast<char>(event.text.unicode);
                if(std::isalpha(enteredChar)|| std::isdigit(enteredChar) || enteredChar == '.' || enteredChar == '_' || enteredChar == '@'){
                    domains[activeBar].write(event);
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && activeBar != -1
             && blank.getElapsedTime().asMilliseconds() >= 50) {
                domains[activeBar].erase(event);
                blank.restart();
            }
        }

        window.clear(sf::Color::White);
        
        for(int i = 0; i < 2; i++){
            // username & password
            domains[i].draw(window, (i == activeBar), darkBlue, basicBlue);

            // Login & Quit
            menuButtons[i].draw(window, pos, darkBlue, basicBlue);

            // hide button
            if(domains[password].hide){
                window.draw(hideViewButton[hide]);
            }else{
                window.draw(hideViewButton[view]);
            }
        }
        // Error Massage
        if(errorStat != -1){
            texts[2].setString(strings[errorStat + 1]);
            window.draw(texts[2]);
        }

        window.display();
        
    }
    return "";
}

void System::renderMenu(std::string user){

    sf::RectangleShape backGround;
    sf::Texture backGroundTexture;
    backGroundTexture.loadFromFile("src/Textures/mainBackGround.png");
    setTexture(backGround, backGroundTexture, sf::Vector2f(0, 0), sf::Vector2f(window.getSize()));

    sf::Text hiMassage;
    setText(hiMassage, sf::Vector2f(scale , 0), sf::Color::White, 0.7 * scale);
    hiMassage.setString("Good Morning, " + user);


    std::string options[] = {
        "Add ", "Delete ", "Modify", "Search", "Withdraw",
        "Deposit", "Transfer", "Report", "Print" , "Quit"
    };

    std::vector<Button> menuButtons;
    for(int i = 0; i < 10; i++){
        menuButtons.push_back(Button(options[i], sf::Vector2f(scale, 1.3 * scale + scale * 3 * i / 4), scale));
    }

    /////////////////////////////// interface members ///////////////////////////////

    enum Error{
        none = -1,
        empty,
        usedNum,
        notFound,
        delBalance,
        limit,
        noBalance,
        sameAcc,
        noTransaction,
        saved
    };
    
    enum Option{
        save, add = 0, del, modify, search, withdraw, deposit, transfer, report, print, Quit, cancel = 8
    };

    enum Members{
        number,
        name,
        phone,
        idx = 2,
        email,
        balance,
        date
    };

    std::string accStrings[] = {"number ", "name   ", "phone  ", "email  ", "balance ", "date   "};
    std::string errorStrs[] = {
        "Fields can't be empty", "This account number is used", "account not found",
        "Can't delete non-zero balance account", "money amount can't exceed 10,000 $",
        "Not enough credit", "Can't Transfer to same account", "No Transactions Yet", "Saved!"
    };
    std::string buttonStrs[] = {
        "Save", "Delete", "Modify", "Search", "Withdraw",
        "Deposit", "Transfer", "Print", "Cancel"
    };

    sf::Text errorMassage, desicionText;
    std::vector<sf::Text> accMembers(6);
    std::vector<sf::Text> reportText(5);
    std::vector<std::vector<InputDomain>> inputDomains(9);
    std::vector<Button> optionButtons, desicionButtons;
    std::deque<std::string> reportStrs;


    /////////////// Texts ///////////////

    // account members Texts
    for(int i = 0; i < 6; i++){
        setText(accMembers[i], sf::Vector2f(scale * 3.5, scale * 1.35 + scale * (i + 1)), sf::Color::White, scale * 0.5);
    }

    // desicion text
    setText(desicionText, sf::Vector2f(scale * 8, scale * 4), sf::Color::White, scale * 0.5);

    // error massage
    setText(errorMassage, sf::Vector2f(scale * 3.5, scale * 7), sf::Color::Red, scale * 0.6);

    // Report
    for(int i = 0; i < 5; i++){
        setText(reportText[i], sf::Vector2f(scale * 3.2, scale * 1.5 + scale * (i + 1)), sf::Color::White, scale * 0.5);
    }


    /////////////// buttons ///////////////

    for(int i = 0; i < 9; i++){
        optionButtons.push_back(Button(buttonStrs[i], sf::Vector2f(scale * 7 + (i == cancel?(scale * 4) : 0), scale * 8), scale));
        optionButtons[i].shape.setSize(sf::Vector2f(scale * 1.75, scale * 0.7));
    }
    optionButtons[search].setPosition(sf::Vector2i(scale * 9, scale * 8), scale);
    optionButtons[withdraw].text.setPosition(sf::Vector2f(scale * 6.4, scale * 8));
    optionButtons[transfer].text.setPosition(sf::Vector2f(scale * 6.4, scale * 8));

    // desicion button
    // Search
    for(int i = 0; i < 2; i++){
        desicionButtons.push_back(Button(accStrings[i], sf::Vector2f(scale * 9 , scale * 5 +  i * (scale)), scale));
        desicionButtons[i].shape.setSize(sf::Vector2f(scale * 1.75, scale * 0.7));
    }
    // Print
    desicionButtons.push_back(Button(accStrings[name], sf::Vector2f(scale * 9 , scale * 5), scale)); // name button
    desicionButtons[2].shape.setSize(sf::Vector2f(scale * 1.75, scale * 0.7));
    for(int i = 4; i < 6; i++){
        desicionButtons.push_back(Button(accStrings[i], sf::Vector2f(scale * 9 , scale * 5 +  (i-3) * (scale)), scale));
        desicionButtons[i-1].shape.setSize(sf::Vector2f(scale * 1.75, scale * 0.7));
    }


    /////////////// options input fields ///////////////
    
    // Add
    for(int i = 0; i < 4; i++){
        inputDomains[add].push_back(InputDomain(accStrings[i]+": ", sf::Vector2f(scale * 3.5, scale * (i+1) * 1.5), scale));
    }
    // Delete
    inputDomains[del].push_back(InputDomain(accStrings[0]+": ", sf::Vector2f(scale * 3.5, scale * 1.5), scale));
    
    // Modify
    for(int i = 0; i < 4; i++){
        inputDomains[modify].push_back(InputDomain(accStrings[i]+": ", sf::Vector2f(scale * 3.5, scale * (i+1) * 1.5), scale));
    }
    // Search
    for(int i = 0; i < 2; i++){
        inputDomains[search].push_back(InputDomain(accStrings[i]+": ", sf::Vector2f(scale * 3.5, scale * 1.5), scale));
    }
    inputDomains[search].push_back(InputDomain("", sf::Vector2f(scale * 12, scale * 3.3), scale,
     sf::Color::White, sf::Color::White, sf::Vector2f(2 * scale, scale)));
    inputDomains[search][2].text.setPosition(sf::Vector2f(scale * 12, scale * 4.3));

    // Withdraw
    inputDomains[withdraw].push_back(InputDomain(accStrings[number]+": ", sf::Vector2f(scale * 3.5, scale * 1.5), scale));
    inputDomains[withdraw].push_back(InputDomain("amount : ", sf::Vector2f(scale * 3.5, scale * 3), scale));
    
    // Deposit
    inputDomains[deposit].push_back(InputDomain(accStrings[number]+": ", sf::Vector2f(scale * 3.5, scale * 1.5), scale));
    inputDomains[deposit].push_back(InputDomain("amount : ", sf::Vector2f(scale * 3.5, scale * 3), scale));
    
    // Transfer
    inputDomains[transfer].push_back(InputDomain(accStrings[number]+": ", sf::Vector2f(scale * 3.5, scale * 1.5), scale));
    inputDomains[transfer].push_back(InputDomain("To\t\t  :  ", sf::Vector2f(scale * 3.5, scale * 3), scale));
    inputDomains[transfer].push_back(InputDomain("amount : ", sf::Vector2f(scale * 3.5, scale * 4.5), scale));
    
    // Report
    inputDomains[report].push_back(InputDomain(accStrings[number]+": ", sf::Vector2f(scale * 3.5, scale * 1.5), scale));\

    // Print
    inputDomains[print].push_back(InputDomain("", sf::Vector2f(scale * 12, scale * 3.3), scale,
     sf::Color::White, sf::Color::White, sf::Vector2f(2 * scale, scale)));
    inputDomains[print][number].text.setPosition(sf::Vector2f(scale * 12, scale * 4.3));
    
    
    std::vector<int> searchResults;
    int activeOption, activeDomain, errorStat, activeAcc, desicion, searchIdx;
    activeOption = activeDomain = errorStat = activeAcc = desicion = searchIdx = none;
    bool typed, modifying = 0, searching, floatPoint = 0;

    sf::Clock blank;


    /////////////////////////////////////////////////////////////////////
    //////////////////////////// render loop ////////////////////////////
    /////////////////////////////////////////////////////////////////////

    while(window.isOpen()){
        sf::Vector2i pos = sf::Mouse::getPosition(window);
        sf::Event event;
        typed = 0;

        bool mouseClicked = (event.type == sf::Event::MouseButtonPressed)
         && (event.mouseButton.button == sf::Mouse::Left);
        

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(mouseClicked){
                for(int i = 0; i < 10; i++){
                    if(menuButtons[i].isTouchingMouse(pos)){
                        activeDomain = errorStat = activeAcc = searchIdx = none;
                        searching = 0;
                        searchResults.resize(0);
                        reportStrs.clear();
                        activeOption = i;
                        break;
                    }
                }

                if(activeOption == add){
                    // set active domain
                    for(int i = 0; i < 4; i++){
                        if(inputDomains[add][i].isTouchingMouse(pos)){
                            activeDomain = i;
                            break;
                        }else{
                            activeDomain = none;
                        }
                    }
                    // Save
                    if(optionButtons[save].isTouchingMouse(pos)){
                        errorStat = none;
                        for(int i = 0; i < 4; i++){
                            if(inputDomains[add][i].input.empty()){
                                errorStat = empty;
                                break;
                            }
                        }
                        if(errorStat != empty && searchAccNum(accounts, strToLL(inputDomains[save][number].input)) != -1){
                            errorStat = usedNum;
                        }
                        if(errorStat == none){
                            accounts.push_back(account(strToLL(inputDomains[add][number].input), inputDomains[add][name].input,
                            inputDomains[add][phone].input, inputDomains[add][email].input));
                            saveToFile(accounts);
                            for(int i = 0; i < 4; i++){
                                inputDomains[add][i].input = "";
                                inputDomains[add][i].inputText.setString("");
                            }
                            errorStat = saved;
                        }
                    }
                    // Cancel
                    if(optionButtons[cancel].isTouchingMouse(pos)){
                        activeOption = activeDomain = errorStat = none;
                        for(int i = 0; i < 4; i++){
                            inputDomains[add][i].input = "";
                            inputDomains[add][i].inputText.setString("");
                        }
                    }
                }
                else if(activeOption == del){
                    // active accNum domain
                    if(inputDomains[del][number].isTouchingMouse(pos)){
                        activeDomain = number;
                    }else{
                        activeDomain = none;
                    }
                    // Search
                    if(optionButtons[search].isTouchingMouse(pos)){
                        errorStat = none;
                        if(inputDomains[del][number].input.empty()){
                            errorStat = empty;
                        }
                        else{
                            activeAcc = searchAccNum(accounts, strToLL(inputDomains[del][number].input));
                            if(activeAcc == -1)
                                errorStat = notFound;
                        }
                    }
                    // Delete
                    if(optionButtons[del].isTouchingMouse(pos) && errorStat == none && activeAcc != none){
                        if(errorStat == none){
                            activeAcc = activeAcc;
                            if(accounts[activeAcc].balance != 0) {
                                errorStat = delBalance;
                                activeAcc = none;
                            }
                            else{
                                accounts.erase(accounts.begin() + activeAcc);
                                saveToFile(accounts);
                                inputDomains[del][number].input = "";
                                inputDomains[del][number].inputText.setString("");
                                activeAcc = none;
                                errorStat = saved;
                            }
                        }
                    }
                    // Cancel
                    if(optionButtons[cancel].isTouchingMouse(pos)){
                        activeOption = activeDomain = errorStat = activeAcc = none;
                        inputDomains[del][number].input = "";
                        inputDomains[del][number].inputText.setString("");
                    }
                }
                else if(activeOption == modify){
                    // set active domain
                    for(int i = 0; i < 4; i++){
                        if(i && !modifying) break;
                        if(inputDomains[modify][i].isTouchingMouse(pos)){
                            activeDomain = i;
                            break;
                        }else{
                            activeDomain = none;
                        }
                    }
                    // Search
                    if(optionButtons[search].isTouchingMouse(pos)){
                        errorStat = activeAcc = none;
                        modifying = 0;
                        for(int i = 1; i < 4; i++){
                            inputDomains[modify][i].input = "";
                            inputDomains[modify][i].inputText.setString("");
                        }
                        if(inputDomains[modify][number].input.empty()){
                            errorStat = empty;
                        }
                        else{
                            activeAcc = searchAccNum(accounts, strToLL(inputDomains[modify][number].input));
                            if(activeAcc == -1)
                                errorStat = notFound;
                        }
                    }
                    // Modify
                    if(optionButtons[modify].isTouchingMouse(pos) && errorStat == none && activeAcc != none && !modifying){
                        // ( .. && !modifying) to avoid overlapping touch of modButton and saveButton
                        modifying = 1;
                        typed = 1;
                        inputDomains[modify][name].input = accounts[activeAcc].name;
                        inputDomains[modify][phone].input = accounts[activeAcc].mobile;
                        inputDomains[modify][email].input = accounts[activeAcc].email;
                        for(int i = 1; i < 4; i++){
                            inputDomains[modify][i].inputText.setString(inputDomains[modify][i].input);
                        }
                    }
                    // Save
                    if(modifying && optionButtons[save].isTouchingMouse(pos) && !typed){
                        errorStat = none;
                        for(int i = 1; i < 4; i++){
                            if(inputDomains[modify][i].input.empty()){
                                errorStat = empty;
                                break;
                            }
                        }
                        if(errorStat == none){
                            accounts[activeAcc].name = inputDomains[modify][name].input;
                            accounts[activeAcc].mobile = inputDomains[modify][phone].input;
                            accounts[activeAcc].email = inputDomains[modify][email].input;
                            saveToFile(accounts);
                            modifying = 0;
                            for(int i = 0; i < 4; i++){
                                inputDomains[modify][i].input = "";
                                inputDomains[modify][i].inputText.setString("");
                            }
                            errorStat = saved;
                        }
                    }
                    // Cancel
                    if(optionButtons[cancel].isTouchingMouse(pos)){
                        activeOption = activeDomain = errorStat = activeAcc = none;
                        modifying = 0;
                        for(int i = 0; i < 4; i++){
                            inputDomains[modify][i].input = "";
                            inputDomains[modify][i].inputText.setString("");
                        }
                    }
                }
                else if(activeOption == search){
                    if(!searching){
                        desicion = search;
                        // make desicion
                        if(desicionButtons[number].isTouchingMouse(pos)){
                            desicion = number;
                            searching = 1;
                        }
                        else if(desicionButtons[name].isTouchingMouse(pos)){
                            desicion = name;
                            searching = 1;
                        }
                    }
                    else{
                        // set active domain
                        if(inputDomains[search][desicion].isTouchingMouse(pos)){
                            activeDomain = desicion;
                        }else if(inputDomains[search][idx].isTouchingMouse(pos)){
                            activeDomain = idx;
                        }else{
                            activeDomain = none;
                        }
                        // Search
                        if(optionButtons[search].isTouchingMouse(pos)){
                            errorStat = activeAcc = none;
                            if(inputDomains[search][desicion].input.empty()){
                                errorStat = empty;
                                searchIdx = none;
                                searchResults.resize(0);
                            }
                            else{
                                switch (desicion)
                                {
                                case number:
                                    activeAcc = searchAccNum(accounts, strToLL(inputDomains[search][number].input));
                                    if(activeAcc == -1){
                                        errorStat = notFound;
                                    }
                                    break;
                                
                                case name:
                                    searchResults = searchAccName(accounts, inputDomains[search][name].input);
                                    if(searchResults.empty()){
                                        errorStat = notFound;
                                        searchIdx = none;
                                        break;
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    // Cancel
                    if(optionButtons[cancel].isTouchingMouse(pos)){
                        activeOption = activeDomain = errorStat = activeAcc = searchIdx = none;
                        searching = 0;
                        for(int i = 0; i < 2; i++){
                            inputDomains[search][i].input = "";
                            inputDomains[search][i].inputText.setString("");
                        }
                    }
                }
                else if(activeOption == withdraw){
                    // set active domain
                    for(int i = 0; i < 2; i++){
                        if(inputDomains[withdraw][i].isTouchingMouse(pos)){
                            activeDomain = i;
                            break;
                        }else{
                            activeDomain = none;
                        }
                    }
                    // Withdraw
                    if(optionButtons[withdraw].isTouchingMouse(pos)){
                        errorStat = none;
                        for(int i = 0; i < 2; i++){
                            if(inputDomains[withdraw][i].input.empty() || strToDouble(inputDomains[withdraw][1].input) == 0){
                                errorStat = empty;
                            }
                        }
                        if(errorStat != empty){
                            activeAcc = searchAccNum(accounts, strToLL(inputDomains[withdraw][number].input));
                            if(activeAcc == none){
                                errorStat = notFound;
                            }else if(strToDouble(inputDomains[withdraw][1].input) > 10000){
                                errorStat = limit;
                            }else if(accounts[activeAcc].balance < strToDouble(inputDomains[withdraw][1].input)){
                                errorStat = noBalance;
                            }else{
                                accounts[activeAcc].balance -= strToDouble(inputDomains[withdraw][1].input);
                                errorStat = saved;
                                saveToFile(accounts);

                                std::string accNum = inputDomains[withdraw][number].input;
                                std::string amount = " " + inputDomains[withdraw][1].input + " $ ";

                                std::string process = options[withdraw] + amount + account::getCurrentDate();
                                registerReport(accNum, process);

                                for(int i = 0; i < 2; i++){
                                    inputDomains[withdraw][i].input = "";
                                    inputDomains[withdraw][i].inputText.setString("");
                                }
                            }
                            activeAcc = none;
                        }
                    }
                    // Cancel
                    if(optionButtons[cancel].isTouchingMouse(pos)){
                        activeOption = activeDomain = errorStat = none;
                        for(int i = 0; i < 2; i++){
                            inputDomains[withdraw][i].input = "";
                            inputDomains[withdraw][i].inputText.setString("");
                        }
                    }
                }
                else if(activeOption == deposit){
                    // set active domain
                    for(int i = 0; i < 2; i++){
                        if(inputDomains[deposit][i].isTouchingMouse(pos)){
                            activeDomain = i;
                            break;
                        }else{
                            activeDomain = none;
                        }
                    }
                    // Deposit
                    if(optionButtons[deposit].isTouchingMouse(pos)){
                        errorStat = none;
                        for(int i = 0; i < 2; i++){
                            if(inputDomains[deposit][i].input.empty() || strToDouble(inputDomains[deposit][1].input) == 0){
                                errorStat = empty;
                            }
                        }
                        if(errorStat != empty){
                            activeAcc = searchAccNum(accounts, strToLL(inputDomains[deposit][number].input));
                            if(activeAcc == none){
                                errorStat = notFound;
                            }else if(strToDouble(inputDomains[deposit][1].input) > 10000){
                                errorStat = limit;
                            }else{
                                accounts[activeAcc].balance += strToDouble(inputDomains[deposit][1].input);
                                errorStat = saved;
                                saveToFile(accounts);

                                std::string accNum = inputDomains[deposit][number].input;
                                std::string amount = " " + inputDomains[deposit][1].input + " $ ";

                                std::string process = options[deposit] + amount + account::getCurrentDate();
                                registerReport(accNum, process);

                                for(int i = 0; i < 2; i++){
                                    inputDomains[deposit][i].input = "";
                                    inputDomains[deposit][i].inputText.setString("");
                                }
                            }
                            activeAcc = none;
                        }
                    }
                    // Cancel
                    if(optionButtons[cancel].isTouchingMouse(pos)){
                        activeOption = activeDomain = errorStat = none;
                        for(int i = 0; i < 2; i++){
                            inputDomains[deposit][i].input = "";
                            inputDomains[deposit][i].inputText.setString("");
                        }
                    }
                }
                else if(activeOption == transfer){
                    // set active domain
                    for(int i = 0; i < 3; i++){
                        if(inputDomains[transfer][i].isTouchingMouse(pos)){
                            activeDomain = i;
                            break;
                        }else{
                            activeDomain = none;
                        }
                    }
                    // Transfer
                    if(optionButtons[transfer].isTouchingMouse(pos)){
                        errorStat = none;
                        for(int i = 0; i < 3; i++){
                            if(inputDomains[transfer][i].input.empty() || strToDouble(inputDomains[transfer][2].input) == 0){
                                errorStat = empty;
                            }
                        }
                        if(errorStat != empty){
                            activeAcc = searchAccNum(accounts, strToLL(inputDomains[transfer][number].input));
                            int toAcc = searchAccNum(accounts, strToLL(inputDomains[transfer][1].input));
                            if(activeAcc == none || toAcc == none){
                                errorStat = notFound;
                            }else if(activeAcc == toAcc){
                                errorStat = sameAcc;
                            }else if(accounts[activeAcc].balance < strToDouble(inputDomains[transfer][2].input)){
                                errorStat = noBalance;
                            }else{
                                accounts[activeAcc].balance -= strToDouble(inputDomains[transfer][2].input);
                                accounts[toAcc].balance += strToDouble(inputDomains[transfer][2].input);
                                errorStat = saved;
                                saveToFile(accounts);

                                std::string from = inputDomains[transfer][number].input;
                                std::string to = inputDomains[transfer][1].input;
                                std::string amount = " " + doubleToString(strToDouble(inputDomains[transfer][2].input)) + "$ ";

                                std::string process = options[transfer] + " to " + to + amount + account::getCurrentDate();
                                registerReport(from, process);

                                process = options[transfer] + " fr. " + from + amount + account::getCurrentDate();
                                registerReport(to, process);
                                floatPoint = 0;

                                for(int i = 0; i < 3; i++){
                                    inputDomains[transfer][i].input = "";
                                    inputDomains[transfer][i].inputText.setString("");
                                }
                            }
                            activeAcc = none;
                        }
                    }
                    // Cancel
                    if(optionButtons[cancel].isTouchingMouse(pos)){
                        activeOption = activeDomain = errorStat = none;
                        floatPoint = 0;
                        for(int i = 0; i < 3; i++){
                            inputDomains[transfer][i].input = "";
                            inputDomains[transfer][i].inputText.setString("");
                        }
                    }
                }
                else if(activeOption == report){
                    // active accNum domain
                    if(inputDomains[report][number].isTouchingMouse(pos)){
                        activeDomain = number;
                    }else{
                        activeDomain = none;
                    }
                    // Report
                    if(optionButtons[report].isTouchingMouse(pos)){
                        errorStat = none;
                        reportStrs.clear();
                        if(inputDomains[report][number].input.empty()){
                            errorStat = empty;
                        }else{
                            activeAcc = searchAccNum(accounts, strToLL(inputDomains[report][number].input));
                            if(activeAcc == none){
                                errorStat = notFound;
                            }else{
                                reportStrs = getReport(inputDomains[report][number].input);
                                if(reportStrs.empty()){
                                    errorStat = noTransaction;
                                }
                            }
                        }
                        activeAcc = none;
                    }
                    // Cancel
                    if(optionButtons[cancel].isTouchingMouse(pos)){
                        activeOption = activeDomain = errorStat = none;
                        reportStrs.clear();
                        inputDomains[report][number].input = "";
                        inputDomains[report][number].inputText.setString("");
                    }
                }
                else if(activeOption == print){
                    if(!searching){
                        desicion = print;
                        // make desicion
                        if(desicionButtons[2].isTouchingMouse(pos)){
                            desicion = name;
                        }
                        else if(desicionButtons[balance - 1].isTouchingMouse(pos)){
                            desicion = balance;
                        }
                        else if(desicionButtons[date - 1].isTouchingMouse(pos)){
                            desicion = date;
                        }
                    }
                    if(desicion != print){
                        searching = 1;
                        // set active domain
                        if(inputDomains[print][number].isTouchingMouse(pos)){
                            activeDomain = number;
                        }
                        // Search
                        switch (desicion)
                        {
                        case name:
                            sortVector(accounts, compareByName);
                            searchResults = searchAccName(accounts, "");
                            break;
                        
                        case balance:
                            sortVector(accounts, compareByBalance);
                            searchResults = searchAccName(accounts, "");
                            break;
                        case date:
                            sortVector(accounts, compareByDate);
                            searchResults = searchAccName(accounts, "");
                            break;
                        }
                    }
                    // Cancel
                    if(optionButtons[cancel].isTouchingMouse(pos)){
                        activeOption = activeDomain = activeAcc = searchIdx = none;
                        searching = 0;
                        inputDomains[print][number].input = "";
                        inputDomains[print][number].inputText.setString("");
                    }
                }
                
                else if(activeOption == Quit)
                {
                    window.close();
                    return;
                }

            }
            // Entering Text
            if (
                event.type == sf::Event::TextEntered &&
                activeOption != none && activeDomain != none &&
                inputDomains[activeOption][activeDomain].input.size() <= 25
            ) {
                char enteredChar = static_cast<char>(event.text.unicode);
                
                if((activeOption == withdraw || activeOption == deposit || activeOption == transfer)){
                    if(inputDomains[activeOption][activeDomain].input.size() > 9) break;
                    if(activeOption == transfer && enteredChar == '.' && !floatPoint){
                        floatPoint = 1;
                    }
                    else if(!std::isdigit(enteredChar)) break;
                }
                else{
                    if(activeDomain == number && (!std::isdigit(enteredChar) || inputDomains[activeOption][number].input.size() > 9)) break;
                    if(activeDomain == name && !(std::isalpha(enteredChar) || enteredChar == ' ')) break;
                    if(activeDomain == phone && !std::isdigit(enteredChar)) break;
                    if(activeDomain == email && !(std::isalpha(enteredChar)|| std::isdigit(enteredChar) || enteredChar == '.' || 
                    enteredChar == '_' || enteredChar == '@')) break;
                }

                inputDomains[activeOption][activeDomain].write(event);
            }
            if (
                sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) &&
                activeDomain != none &&
                blank.getElapsedTime().asMilliseconds() >= 50
            ) {
                if(activeOption == transfer && floatPoint){
                    int lastCharIdx =  inputDomains[activeOption][activeDomain].input.size() - 1;
                    if(inputDomains[activeOption][activeDomain].input[ lastCharIdx ] == '.')
                        floatPoint = 0;
                }
                inputDomains[activeOption][activeDomain].erase(event);
                blank.restart();
            }
        }

        window.clear(sf::Color::White);
        window.draw(backGround);
        window.draw(hiMassage);
        
        for(int i = 0; i < 10; i++){
            if(activeOption == i){
                menuButtons[i].draw(window, pos, pressedButton, pressedButton);
            }
            else{
                menuButtons[i].draw(window, pos, sf::Color::Blue, basicBlue);
            }
        }
        
        // error massage
        if(errorStat != none){
            errorMassage.setString(errorStrs[errorStat]);
            window.draw(errorMassage);
        }


        if(activeOption != none){
            // Cancel Button
            optionButtons[cancel].draw(window, pos, darkBlue, activeBlue);


            if(searchResults.size() && activeDomain != none &&(strToLL(inputDomains[activeOption][activeDomain].input)) <= searchResults.size()){
                searchIdx = strToLL(inputDomains[activeOption][activeDomain].input) - 1;
            }
            // draw acc members
            if(activeAcc != none && !modifying || searchIdx != none){
                if(searchIdx != none) activeAcc = searchResults[searchIdx];
                accMembers[number].setString(accStrings[number]+": "+ numToString(accounts[activeAcc].accNumber));
                accMembers[name].setString(accStrings[name]+": "+ accounts[activeAcc].name);
                accMembers[phone].setString(accStrings[phone]+": "+ accounts[activeAcc].mobile);
                accMembers[email].setString(accStrings[email]+": "+ accounts[activeAcc].email);
                if(accounts[activeAcc].balance){
                    accMembers[balance].setString(accStrings[balance]+": "+ doubleToString(accounts[activeAcc].balance)+" $");
                }else{
                    accMembers[balance].setString(accStrings[balance]+": 0 $");
                }
                accMembers[date].setString(accStrings[date]+": "+ accounts[activeAcc].dateToPrint());
                for(int i = 0; i < 6; i++){
                    if(i == date && errorStat == saved) break;
                    window.draw(accMembers[i]);
                }
            }
            
            switch (activeOption)
            {
                case add:
                    // acc members input fields
                    for(int i = 0; i < 4; i++){
                        inputDomains[add][i].draw(window, (i == activeDomain), basicBlue, activeBlue);
                    }
                    // save button
                    optionButtons[save].draw(window, pos, darkBlue, activeBlue);

                    break;
                case del:
                    // acc num input field
                    inputDomains[del][save].draw(window, (number == activeDomain), basicBlue, activeBlue);

                    // delete & search menuButtons
                    optionButtons[search].draw(window, pos, basicGreen, activeGreen);
                    if(activeAcc != none)
                        optionButtons[del].draw(window, pos, sf::Color::Red, activeRed);

                    break;
                case modify:
                    // acc members input fields
                    for(int i = 0; i < 4; i++){
                        if(i && !modifying) break;
                        inputDomains[modify][i].draw(window, (i == activeDomain), basicBlue, activeBlue);
                    }

                    // save & modify & search menuButtons
                    optionButtons[search].draw(window, pos, basicGreen, activeGreen);
                    if(modifying){
                        optionButtons[save].draw(window, pos, darkBlue, activeBlue);
                    }else if(activeAcc != none){
                        optionButtons[modify].draw(window, pos, darkBlue, activeBlue);
                    }

                    break;
                case search:
                    // 'search by' buttons & desicion Text
                    if(!searching){
                        desicionText.setString(options[desicion]+" by: ");
                        window.draw(desicionText);
                        for(int i = number; i <= name; i++){
                            desicionButtons[i].draw(window, pos, basicGreen, activeGreen);
                        }
                    }
                    else {
                        // input field
                        inputDomains[search][desicion].draw(window, (desicion == activeDomain), basicBlue, activeBlue);
                        // search button
                        optionButtons[search].draw(window, pos, basicGreen, activeGreen);

                        // Idx square
                        inputDomains[search][idx].text.setString(numToString(searchResults.size()));
                        if(desicion == name){
                            inputDomains[search][idx].input = numToString(searchIdx+1);
                            inputDomains[search][idx].inputText.setString(inputDomains[search][idx].input);
                            inputDomains[search][idx].draw(window, activeDomain == 2, basicBlue, activeBlue);
                        }
                    }

                    break;
                case withdraw:
                    for(int i = 0; i < 2; i++){
                        inputDomains[withdraw][i].draw(window, (i == activeDomain), basicBlue, activeBlue);
                    }
                    
                    optionButtons[withdraw].draw(window, pos, darkBlue, activeBlue);

                    break;
                case deposit:
                    for(int i = 0; i < 2; i++){
                        inputDomains[deposit][i].draw(window, (i == activeDomain), basicBlue, activeBlue);
                    }
                    
                    optionButtons[deposit].draw(window, pos, darkBlue, activeBlue);

                    break;
                case transfer:
                    for(int i = 0; i < 3; i++){
                        inputDomains[transfer][i].draw(window, (i == activeDomain), basicBlue, activeBlue);
                    }
                    
                    optionButtons[transfer].draw(window, pos, darkBlue, activeBlue);

                    break;
                case report:
                    inputDomains[report][number].draw(window, (number == activeDomain), basicBlue, activeBlue);
                    
                    optionButtons[report].draw(window, pos, darkBlue, activeBlue);
                    if(!reportStrs.empty()){
                        for(int i = 0; i < reportStrs.size(); i++){
                            reportText[i].setString(reportStrs[i]);
                            window.draw(reportText[i]);
                        }
                    }

                    break;
                case print:
                    // 'search by' buttons & desicion Text
                    if(!searching){
                        desicionText.setString(options[desicion]+" by: ");
                        window.draw(desicionText);
                        desicionButtons[2].draw(window, pos, basicGreen, activeGreen);
                        for(int i = balance - 1; i <= date -1; i++){
                            desicionButtons[i].draw(window, pos, basicGreen, activeGreen);
                        }
                    }
                    else {
                        // Idx square
                        inputDomains[print][number].text.setString(numToString(searchResults.size()));
                        inputDomains[print][number].input = numToString(searchIdx+1);
                        inputDomains[print][number].inputText.setString(inputDomains[print][number].input);
                        inputDomains[print][number].draw(window, activeDomain == number, basicBlue, activeBlue);
                    }

                    break;
            }
        }
        
        window.display();
    }
}


void System::run(){

    if(window.getSize() == sf::Vector2u(1, 1)){
        window.close();
        return;
    }

    // std::string user = "Luka";
    std::string user = renderLogin(); if(user == "") return;
    renderMenu(user);

    return;
}
