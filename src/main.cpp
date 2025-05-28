#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>


float Pquicar = 0.1;
float Patrito = 1;
float Pmultiplicador = 0.000042;
float PnumeroParticulas = 10;
float Ppuxar = 100;
float Pempurrar = 24;
float Pvelocidade = 0.5;
float PwAtrito = 0.5;

static float random(
int minimo, int maximo) {
    // Inicializa um gerador de números aleatórios com uma semente não determinística
    std::random_device rd;
    std::mt19937 gen(rd());

    // Distribuição uniforme entre minimo e maximo
    std::uniform_real_distribution<double> dis(minimo, maximo);

    return dis(gen);
}


class Particle {
private:
    int radius;
    bool isMoving = true;
    sf::Color colorDestaque;

public:
    bool destaque = false;
    float x;
    float y;
    float velocidadeX = 0;
    float velocidadeY = 0;
    float aceleracaoX = 0.3;
    float aceleracaoY = 0.5;
	int conexoes = 0;
	int limiteConexoes = 4;
    float empurrar = 24;
	sf::Font font;

    Particle(int radius, float x, float y) : radius(radius), x(x), y(y) {

        if (!font.openFromFile("Roboto-Regular.ttf"))
        {
            std::cerr << "Erro ao carregar a fonte" << std::endl;
            return;
        }
        colorDestaque = sf::Color::Red;
    }


    void move(float widthTela, float heightTela) {
        if (!isMoving)  return;




        velocidadeX += aceleracaoX;
        velocidadeY += aceleracaoY;

        aceleracaoX = 0;
        aceleracaoY = 0;
        velocidadeX *= Patrito; // atrito
        velocidadeY *= Patrito; // atrito
        x += velocidadeX;
        y += velocidadeY;

        std::unordered_map <std::string, float> limites = {
            {"esquerda", radius / 2},
            {"direita", widthTela - radius},
            {"cima", (y < radius ? 1 : 0) / 2 },
            {"baixo", heightTela - radius}
        };

        if (x < limites["esquerda"]) {
            x += 1.1 * (limites["esquerda"] - x);
            velocidadeX *= -Pquicar;
            velocidadeX *= PwAtrito;
        }
        else if (x > limites["direita"]) {
            x += 1.1 * (limites["direita"] - x);
            velocidadeX *= -Pquicar;
            velocidadeY *= PwAtrito;
        }

        if (y < limites["cima"]) {
            y += 1.1 * (limites["cima"] - y);
            velocidadeY *= -Pquicar;
            velocidadeY *= PwAtrito;
        }
        else if (y > limites["baixo"]) {
            y += 1.1 * (limites["baixo"] - y);
            velocidadeY *= -Pquicar;
            velocidadeY *= PwAtrito;
        }

        int areaEmpurro = radius + 100;
        float aceleracao = 0.08;

        if (x < areaEmpurro) aceleracaoX += aceleracao;
        if (x > limites["direita"] - areaEmpurro) aceleracaoX -= aceleracao;

        if (y < areaEmpurro) aceleracaoY += aceleracao;
        if (y > limites["baixo"] - areaEmpurro) aceleracaoY -= aceleracao;



    }

    float distanciaEntre(Particle& outra) const {
        return (sqrt(pow(x - outra.x, 2) + pow(y - outra.y, 2)));
    }

    bool intersecao(Particle& outra) const {
        float distancia = distanciaEntre(outra);
        std::cout << "Distância entre partículas: " << distancia << std::endl;
        return distancia < (radius + outra.radius);
    }


    void show(sf::RenderWindow& window) {
        sf::CircleShape shape(radius, 20);
        shape.setPosition({ x, y });

        if (destaque == true) {
            shape.setFillColor(sf::Color::Red);
        }
        else {
            shape.setFillColor(sf::Color::Green);
        }



        sf::Text text(font); // a font is required to make a text object

        // set the string to display
        text.setString("X: " + std::to_string(static_cast<int>(x)) + " Y: " + std::to_string(static_cast<int>(y)));

        // set the character size
        text.setCharacterSize(12); // in pixels, not points!
        text.setPosition({ x + radius, y });

        // set the color
        text.setFillColor(sf::Color::Green);

		window.draw(text);        
        window.draw(shape);
    }

};


std::vector<Particle> CriarParticles() {
	std::vector<Particle> particles;
	for (int i = 0; i < 10; i++) {
		particles.push_back(Particle(10, random(0, 600), random(0, 600)));
	}

	return particles;
}

int main()
{
        

    auto window = sf::RenderWindow(sf::VideoMode({1000u, 1000u}), "CMake SFML Project");
    window.setFramerateLimit(144);
        
	std::vector<Particle> particles = CriarParticles();



    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();


		for (auto& particle : particles) {
        
            for (auto& particle2 : particles) {
                if (&particle == &particle2) continue;


                if (particle.intersecao(particle2)) {
                    particle.destaque = true;
                }
                else {
                    particle.destaque = false;

                }

            }
            
            
            particle.show(window);
            particle.move(1000, 1000);

		}

//		for (auto& conexao : conexoes) {
	//		conexao.show(window);
		//}
       
		window.display();
    }
}
