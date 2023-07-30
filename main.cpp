#include <iostream>
#include <vector>
#include <any>
#include <tuple>
#include <SFML/Graphics.hpp>
#include <random>



class Game
{
private:
    

public:
    sf::Sprite sprite;
    sf::Text text;
    sf::Font font;
    std::vector<std::vector<std::vector<std::vector<std::any>>>> q_table;
    std::vector<std::string> actions = {"flap", "glide"};

    void create_q_table(){
    
    for (int h = 0 ;h <= 640;h++){ // Y_COORDINATES
        std::vector<std::vector<std::vector<std::any>>> d0 = {};
        q_table.push_back(d0);
        for (int x = 0; x <= 275; x++){ // CLOSEST PIPE X COORD
            std::vector<std::vector<std::any>> d1 = {};
            q_table.back().push_back(d1);
            for (int y = 286; y <= 416; y++){ // Y PIPE (130)
                std::vector<std::any> v1 = {0.f, 0.f, h, x, y};
                q_table.back().back().push_back(v1);

            }
        }
    } 
     
    }

    Game(const sf::Texture& texture, const sf::Vector2f& position)
    {   
        if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf")){
            throw std::runtime_error("font failed to load");
        }

        sprite.setTexture(texture);
        sprite.setScale(1.8, 1.5);
        sprite.setPosition(position);
    }

    void moveSprite(const sf::Vector2f& offset)
    {
        sf::Vector2f newPosition = sprite.getPosition() - offset;
        sprite.setPosition(newPosition);

        if (sprite.getPosition().x + 510 <= 0){
            sf::Vector2f newPosition = sf::Vector2f(479.f, 0.f);
            sprite.setPosition(newPosition);
        }

    }

    void draw(sf::RenderWindow& window)

    {   
        window.draw(sprite);
               
    }


};


class Bird{

    private:
        float drop_speed = 0.005f;
        float decay = 1.f;
        float rotate = 0.f;
        sf::Texture birdu1;
        std::vector<sf::Texture> txtures;

    public:
        std::random_device rd;
        float y;
        float x = 50.f;
        float og_x = x;
        sf::Sprite sprite;
        int choice;
        

        
        Bird(){
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float>ep(320,440);
            y = ep(gen);
            txtures = load_birds();
            sprite.setTexture(txtures[1]);
            y -= (sprite.getLocalBounds().height*1.4);
            sprite.setPosition(sf::Vector2f(x, y));
            sprite.setScale(sf::Vector2f(1.4,1.4));
            sprite.setRotation(rotate);
                       

        }

        void draw(sf::RenderWindow& window){
            window.draw(sprite);
        }

        void fly(int fly){
            if (fly == 1){
                decay = 0;
                y -= 25.f; //was 25.f for non ai.
                rotate = -20.f;
                sprite.setTexture(txtures[2]);
                sprite.setPosition(sf::Vector2f(x, y));
            } else if (fly == 0){
                y += drop_speed * decay;
                decay += 0.0015f;

                if (rotate < 50){
                   rotate += 0.008;     
                }
                else { 
                    rotate = 50;
                }
                
                if (decay >= 1.5 && decay <= 2.5){
                    sprite.setTexture(txtures[1]);
                } else if (decay > 2.5){
                    sprite.setTexture(txtures[0]);
                }
                sprite.setRotation(rotate);
                sprite.setPosition(sf::Vector2f(x, y));
            }


        }

        std::vector<sf::Texture> load_birds(){

            std::vector<sf::Texture> txt;

            sf::Texture texture2;
            if (!texture2.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/bird1.png")){
                throw std::runtime_error("pipe image not found");
            }
            txt.push_back(texture2);

            sf::Texture texture3;
            if (!texture3.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/bird2.png")){
                throw std::runtime_error("pipe image not found");
            }
            txt.push_back(texture3);

            sf::Texture texture4;
            if (!texture4.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/bird3.png")){
                throw std::runtime_error("pipe image not found");
            }
            txt.push_back(texture4);

            return txt;
            
        } 


        void compare_qvalues(std::vector<std::any> v1){                       
            std::any val0 = v1[0];
            float i_val0 = std::any_cast<float>(val0);
            std::any val1 = v1[1];
            float i_val1 = std::any_cast<float>(val1);

            if (i_val0 > i_val1){
                choice = 0;         
            } else if (i_val0 < i_val1) {
                choice = 1;
            } else if (i_val0 == i_val1) {
                choice = 0;
              }

            

        }


        void decide(std::vector<std::any> v1, float epsilon){
            int pre_choice; 
            std::mt19937 gen(rd());
            std::default_random_engine engine(rd());
            std::uniform_real_distribution<float>ep(0.f,1.f);
            std::uniform_int_distribution<int>iinntt(0,1);
            
            if(ep(gen) < epsilon){
                choice = iinntt(engine);
                //std::cout << "here decide." << std::endl;
            }

            else {
                compare_qvalues(v1);
            }

            fly(choice);
               
        } 

        void change_choice(int ffllyy){
            choice = ffllyy;
        } 

        void reset(){
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float>ep(320,440);
            y = ep(gen);
            x = og_x;
            rotate = 0.f;
            decay = 1.f;
            sprite.setRotation(rotate);
            
        }
        
};


class Pipe{

    private:
        std::random_device rd;  // Obtain a random seed from the system
        sf::Texture pipeimg;
        
        
        
    public:
        std::vector<float> closest_pipe;
        sf::Sprite pos2dieU;
        sf::Sprite pos2dieB;
        sf::Texture diepos;
        sf::Sprite pipex1U;
        sf::Sprite pipex1B;
        sf::Sprite pipex2U;
        sf::Sprite pipex2B;
        float x1;
        float x2;
        float y1;
        float y2; 
        float og_x1;
        float og_x2; 
        
        Pipe(){

            if (!pipeimg.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/pipe.png")){
                throw std::runtime_error("pipe image not found");
            }

            if (!diepos.loadFromFile("/home/kali/Pictures/red_dot.png")){
                throw std::runtime_error("dot image not found");
            }

            pos2dieU.setTexture(diepos); 
            pos2dieB.setTexture(diepos);          
            pipex1U.setTexture(pipeimg);
            pipex1B.setTexture(pipeimg);
            pipex2U.setTexture(pipeimg);
            pipex2B.setTexture(pipeimg);
            pipex1U.setScale(sf::Vector2f(1.f,1.3f));
            pipex2U.setScale(sf::Vector2f(1.f,1.3f));
            pipex1B.setScale(sf::Vector2f(1.f,1.3f));
            pipex2B.setScale(sf::Vector2f(1.f,1.3f));

            //std::cout << "pipex1U.getGlobalBounds().height = " << pipex1U.getGlobalBounds().height;
                       
            
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float>why1(pipex1U.getGlobalBounds().height -130.f,pipex1U.getGlobalBounds().height);
            std::uniform_real_distribution<float>why2(pipex1U.getGlobalBounds().height - 80.f,pipex1U.getGlobalBounds().height);
            std::uniform_real_distribution<float>ex1(80,275);
            x1 = ex1(gen);
            x2 = x1 + 200;
            y1 = why1(gen);
            y2 = why2(gen);
            closest_pipe.push_back(x1);
            closest_pipe.push_back(y1);
            closest_pipe.push_back(y1 + 120.f);

            //pos2dieU.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[1]));
            //pos2dieB.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[2]));
            pipex1U.setPosition(sf::Vector2f(x1, y1));
            pipex1U.setRotation(180.f);
            pipex1B.setPosition(sf::Vector2f(x1 - pipex1B.getGlobalBounds().width, y1 + 120.f));
            
            pipex2U.setPosition(sf::Vector2f(x2, y2));
            pipex2U.setRotation(180.f);
            pipex2B.setPosition(sf::Vector2f(x2 - pipex2B.getGlobalBounds().width, y2 + 120.f));

            //std::cout << "pipex1U.getGlobalBounds().height is: " << pipex1U.getGlobalBounds().height << std::endl;
            //std::cout << "pipex1U.getGlobalBounds().height is: " << pipex1U.getGlobalBounds().height - 130 << std::endl;

        }

        void draw(sf::RenderWindow& window){
            window.draw(pipex1U);
            window.draw(pipex1B);
            window.draw(pipex2U);
            window.draw(pipex2B);
            //window.draw(pos2dieU);
            //window.draw(pos2dieB);
        }

        void move(float speed){ //0.02
            x1 -= speed;
            x2 -= speed;

            pipex1U.setPosition(sf::Vector2f(x1, y1));
            pipex1B.setPosition(sf::Vector2f(x1 - pipex1B.getGlobalBounds().width, y1 + 120.f));
            
            pipex2U.setPosition(sf::Vector2f(x2, y2));
            pipex2B.setPosition(sf::Vector2f(x2 - pipex2B.getGlobalBounds().width, y2 + 120.f));

            spawn();    
            //pos2dieU.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[1]));
            //pos2dieB.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[2]));
        }

        void spawn(){
            
            if (x1 - pipex1U.getGlobalBounds().width <= 0.f){
                std::mt19937 gen(rd());
                std::uniform_real_distribution<float>why1(pipex1U.getGlobalBounds().height -130.f,pipex1U.getGlobalBounds().height);
                x1 += 400.f;
                y1 = why1(gen);
                pipex1U.setPosition(sf::Vector2f(x1, y1));
                pipex1B.setPosition(sf::Vector2f(x1 - pipex1B.getGlobalBounds().width, y1 + 120.f));  
            }

            if (x2 - pipex1U.getGlobalBounds().width <= 0.f){
                std::mt19937 gen(rd());
                std::uniform_real_distribution<float>why2(pipex1U.getGlobalBounds().height - 80.f,pipex1U.getGlobalBounds().height);
                x2 += 400.f;
                y2 = why2(gen);
                pipex2U.setPosition(sf::Vector2f(x2, y2));
                pipex2B.setPosition(sf::Vector2f(x2 - pipex2B.getGlobalBounds().width, y2 + 120.f));
            }

            if (x1 < x2){
                closest_pipe[0] = x1;
                closest_pipe[1] = y1;
                closest_pipe[2] = y1+120.f;

            } else if (x2 < x1){
                closest_pipe[0] = x2;
                closest_pipe[1] = y2;
                closest_pipe[2] = y2+120.f;    
            }
        }

        void reset(){
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float>ex1(80,275);
            std::uniform_real_distribution<float>why1(pipex1U.getGlobalBounds().height -130.f,pipex1U.getGlobalBounds().height);
            std::uniform_real_distribution<float>why2(pipex1U.getGlobalBounds().height - 80.f,pipex1U.getGlobalBounds().height);
            x1 = ex1(gen);
            x2 = x1 + 200; 
            y1 = why1(gen);
            y2 = why2(gen);
            closest_pipe.push_back(x1);
            closest_pipe.push_back(y1);
            closest_pipe.push_back(y1 + 120.f);

            //pos2dieU.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[1]));
            //pos2dieB.setPosition(sf::Vector2f(closest_pipe[0], closest_pipe[2]));
            pipex1U.setPosition(sf::Vector2f(x1, y1));
            pipex1U.setRotation(180.f);
            pipex1B.setPosition(sf::Vector2f(x1 - pipex1B.getGlobalBounds().width, y1 + 120.f));
            
            pipex2U.setPosition(sf::Vector2f(x2, y2));
            pipex2U.setRotation(180.f);
            pipex2B.setPosition(sf::Vector2f(x2 - pipex2B.getGlobalBounds().width, y2 + 120.f));
        }    


};



std::vector<sf::Texture> init_textures(){
    std::vector<sf::Texture> textures;
    //std::cout << "textures.size() = " << textures.size();

    sf::Texture texture;
    if (!texture.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/bg.png")){
        throw std::runtime_error("background image not found");
    }
    textures.push_back(texture);

    sf::Texture texture1;
    if (!texture1.loadFromFile("/home/kali/Desktop/machine_learning/reinforcement_learning/imgs/pipe.png")){
        throw std::runtime_error("pipe image not found");
    }
    textures.push_back(texture1);

    return textures;
}; 




int main()

{
    std::vector<sf::Texture> textures = init_textures();
    std::vector<std::any> current_q_vec; 
    std::vector<std::any> next_q_vec; 
    Game Background(textures[0], sf::Vector2f(0.f, 0.f));
    Background.create_q_table();
    Game Background2(textures[0], sf::Vector2f(479.f,0.f));
    sf::RenderWindow window(sf::VideoMode(480, 640), "SFML Window");
    Bird bird;
    Pipe pipe;
    sf::Text text;
    sf::Text ep_rate;
    sf::Font font;
    int score = 0;
    int itter = 0; 
    float epsilon = 1.f;
    float im_reward; 
    float new_q_value;
    float alpha = 1.f;
    float gamma = 0.5f; 
    float cur_val;
    float next_max;
    int agent_op = 1;
    float speed_pipe = 0.02;
    float speed_back = 0.05;
    int fly = 0;
    int q_table_index0;
    int q_table_index1;
    int bird_y;
    int reset = 0;

    if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf")){
            throw std::runtime_error("font failed to load");
    }

    //SET THE TEXTS. 
    text.setString("Score: " + std::to_string(score));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);
    text.setPosition(30,15);
    text.setFont(font);

    ep_rate.setString("Epsilon: " + std::to_string(epsilon));
    ep_rate.setCharacterSize(24);
    ep_rate.setFillColor(sf::Color::Black);
    ep_rate.setPosition(30,35);
    ep_rate.setFont(font);


    while (window.isOpen())

    {
        sf::Event event;
        fly = 0;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            //if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){
                //bird.fly(1);
            //} 

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E){
                epsilon += 0.1f;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O){
                epsilon = 0.f;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A){
                agent_op = 1;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){
                fly = 1;
                //std::cout << std::any_cast<float>(next_q_vec[0]) << std::endl;
                //std::cout << std::any_cast<float>(next_q_vec[1]) << std::endl;
                std::cout << "bird.y is " << bird.y << " std::any_cast<int>(next_q_vec[2]) is: " <<std::any_cast<int>(next_q_vec[2]) << std::endl;
                std::cout << "pipe.closest_pipe[0] is " << pipe.closest_pipe[0] << " std::any_cast<int>(next_q_vec[3]) is: " <<std::any_cast<int>(next_q_vec[3]) << std::endl;
                std::cout << "pipe.closest_pipe[1] is " << pipe.closest_pipe[1] << " std::any_cast<int>(next_q_vec[4]) is: " <<std::any_cast<int>(next_q_vec[4]) << std::endl;
                
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S){
                agent_op = 0;
            } 

            

        }

                
        if (epsilon <= 0.f){
            epsilon = 0.f;
        }        

        
        // MOVE ENVIRONMENT
        
        bird.fly(0);
        pipe.move(speed_pipe);
        Background.moveSprite(sf::Vector2f(speed_back, 0.f));
        Background2.moveSprite(sf::Vector2f(speed_back, 0.f));

        int closest_pipe_0 = std::round(pipe.closest_pipe[0]);
        int closest_pipe_1 = std::round(pipe.closest_pipe[1]);
        
        //Y_COORDINATES BIRD CLOSEST PIPE X Y PIPE
        q_table_index0 = static_cast<int>(closest_pipe_0);
        q_table_index1 = static_cast<int>(closest_pipe_1) - 286;
        bird_y = static_cast<int>(bird.y);


        //CREATE STATE OBJECT AND CHECK THE STATE
        current_q_vec = Background.q_table[bird_y][q_table_index0][q_table_index1];
        /*if (itter >= 1000){
            std::cout << "\n\nbird_y is: " << bird_y << std::endl;
            std::cout << "pipe x is: " << pipe.closest_pipe[0] << std::endl;
            std::cout << "pipe y is: " << pipe.closest_pipe[1] << std::endl;

            std::cout << "next q vec state is: -------------------------\n";
            std::cout << "value at index 2 is: " << std::any_cast<int>(current_q_vec[2]) << std::endl; 
            std::cout << "value at index 3 is: " << std::any_cast<int>(current_q_vec[3]) << std::endl; 
            std::cout << "value at index 4 is: " << std::any_cast<int>(current_q_vec[4]) << std::endl;         
            
        }*/

        //DECIDE
        if (agent_op == 0){
            bird.decide(current_q_vec,epsilon);  
        } else {
            bird.change_choice(fly);
            bird.fly(bird.choice);
            //std::cout << "bird.choice is: " << bird.choice << std::endl;
            //std::cout << "not flew 0" <<std::endl;
        }
        

        // RESET CHECK / IM REWARD CHECK.
        if (bird.y < pipe.closest_pipe[1] && bird.x + bird.sprite.getGlobalBounds().width > pipe.closest_pipe[0] - pipe.pipex2B.getGlobalBounds().width && bird.x < pipe.closest_pipe[0] || bird.y <= 25.f || bird.y > 640.f || bird.y + bird.sprite.getGlobalBounds().height > pipe.closest_pipe[2] && bird.x + bird.sprite.getGlobalBounds().width > pipe.closest_pipe[0] - pipe.pipex2B.getGlobalBounds().width && bird.x < pipe.closest_pipe[0]){
            reset = 1;
            score = 0;
            im_reward = -100.f;
            //std::cout << "reward of -100" << std::endl;
            
        }


        else if (pipe.closest_pipe[0] - pipe.pipex1U.getGlobalBounds().width <= 0.02f){           
                score += 1;  
                im_reward = 100.f;  
                //std::cout << "reward of 100" << std::endl;
 
        }

        else {
            im_reward = 1.f;
        }


               


        //RETREIVE THE CUR Q VAL FOR THE OG STATE                   HERE <<<<< 
        cur_val = std::any_cast<float>(current_q_vec[bird.choice]);
        //std::cout << "\nval0 = " << val0 << " val1 = " << val1 << "\n\n"; 
        //std::cout << "cur_max is: " << cur_max << "\n\n";


        //GET POST DECIDE Q-TABLE STATE CHECK THE STATE
        int post_closest_pipe_0 = std::round(pipe.closest_pipe[0]);
        int post_closest_pipe_1 = std::round(pipe.closest_pipe[1]);  
        int post_q_table_index0 = static_cast<int>(post_closest_pipe_0);
        int post_q_table_index1 = static_cast<int>(post_closest_pipe_1) - 286;
        int post_bird_y = static_cast<int>(bird.y);
        next_q_vec = Background.q_table[post_bird_y][post_q_table_index0][post_q_table_index1];
 
        //RETREIVE THE MAX Q VAL FOR THE NEXT STATE
        float post_val0 = std::any_cast<float>(next_q_vec[0]);
        float post_val1 = std::any_cast<float>(next_q_vec[1]);
        //std::cout << "\nval0 = " << val0 << " val1 = " << val1 << "\n\n";

        if (post_val0 > post_val1){
            next_max = post_val0;
        } else if (post_val1 > post_val0) {
            next_max = post_val1;
        } else{
            next_max = std::any_cast<float>(next_q_vec[bird.choice]);
        }
        //std::cout << "cur_max is: " << next_max << "\n\n";
        //std::cout << "bird.y is: " << bird.y << std::endl; 
        //std::cout << "bird_y is: " << bird.y << std::endl;
        //UPDATE THE Q VALUES

        //alpha = 1 | gamma = 0.5 
        new_q_value = cur_val + alpha * (im_reward + (gamma * next_max)) - cur_val;
        //std::cout << "\n\nnew_q_value = "  << new_q_value << std::endl;
        Background.q_table[bird_y][q_table_index0][q_table_index1][bird.choice] = new_q_value;
        std::any nw = Background.q_table[bird_y][q_table_index0][q_table_index1][bird.choice];
        //float nw_i = std::any_cast<float>(nw);
        //std::cout << "new background table value is: " << nw_i;

        /*if (fly == 1){
            std::cout << "----------------------------\n" << std::endl;
            std::cout << "bird_y = " << bird_y << std::endl;
            std::cout << "q_table_index0 = " << q_table_index0 << std::endl;
            std::cout << "q_table_index1 = " << q_table_index1 << std::endl;
            std::cout << "current_q_vec[4] = " << std::any_cast<int>(current_q_vec[4]) << std::endl;
            std::any nw = Background.q_table[bird_y][q_table_index0][q_table_index1][bird.choice];
            float nw_i = std::any_cast<float>(nw);
            std::cout << "\n----------------------------\n" << std::endl;
            std::cout << "cur_val = " << cur_val << std::endl;
            std::cout << "next_max = " << next_max << std::endl;
            std::cout << "im_reward = " << im_reward << std::endl;
            std::cout << "bird.choice is: " << bird.choice << std::endl; 
            std::cout << "new background table value is: " << nw_i << "\n\n" <<std::endl;
            
        }*/
        

        // DRAW
        text.setString("Score: " + std::to_string(score));
        ep_rate.setString("Epsilon: " + std::to_string(epsilon));
        window.clear();
        Background.draw(window);
        Background2.draw(window);
        pipe.draw(window);
        bird.draw(window);
        window.draw(text);
        window.draw(ep_rate);
        window.display();

        epsilon -= 0.0000000228f;
        itter += 1;    

        if (reset == 1){
            bird.reset();
            pipe.reset();
            reset = 0;
            //std::cout << "here reset";
        }   
                
    }

    return 0;
}
