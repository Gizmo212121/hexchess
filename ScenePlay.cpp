#include "ScenePlay.h"
#include "SceneMenu.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Joystick.hpp>
#include <fstream>
#include <memory>
#include <unordered_map>

ScenePlay::ScenePlay(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine), m_levelPath(levelPath)
{
    init(m_levelPath);
}

void ScenePlay::init(const std::string& levelPath)
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");
    registerAction(sf::Keyboard::A, "MOVE_LEFT");
    registerAction(sf::Keyboard::D, "MOVE_RIGHT");
    registerAction(sf::Keyboard::S, "DUCK");
    registerAction(sf::Keyboard::W, "JUMP");
    registerAction(sf::Keyboard::Space, "SWING_SWORD");


    // TODO: REGISTER ALL OTHER GAMEPLAY ACTIONS

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("FreeSerif"));

    loadLevel(levelPath);
}

void ScenePlay::loadLevel(const std::string& path)
{
    m_entityManager = EntityManager();

    // TODO: read in the level file and add the appropriate entities
    //       use the PlayerConfig struct m_playerConfig to store player properties
    //       this struct is defined at the top of Scene_Play.h

    std::ifstream fin;

    fin.open(path);

    if (!fin.is_open())
    {
        std::cerr << "Error: Could not read or open file!\n";
        return;
    }

    std::string dataIn;

    while (fin >> dataIn)
    {
        if (dataIn == "Player")
        {
            std::cout << "Loading player data\n";
            float spawnX, spawnY, grav , maxSpeed, movementSpeed, damping, jump, framesToDamage;

            fin >> spawnX >> spawnY >> grav >> maxSpeed >> movementSpeed >> damping >> jump >> framesToDamage;

            m_playerConfig.SPAWN_X = spawnX;
            m_playerConfig.SPAWN_Y = spawnY;
            m_playerConfig.GRAVITY = grav;
            m_playerConfig.MAXSPEED = maxSpeed;
            m_playerConfig.SPEED = movementSpeed;
            m_playerConfig.DAMPING = damping;
            m_playerConfig.JUMP = jump;
            m_playerConfig.FRAMES_TO_DAMAGE = framesToDamage;
        }
        else if (dataIn == "Tile")
        {
            std::cout << "Loading tile data\n";
            std::string name; // needs to be valid animation asset
            float x, y;

            fin >> name >> x >> y;

            const std::shared_ptr<Entity>& entity = m_entityManager.addEntity("Tile");
            const std::shared_ptr<Animation>& animation = m_game->assets().getAnimation(name);

            if (!entity->hasComponent<CAnimation>())
            {
                std::cout << "Adding animation of name " << name << std::endl;
                entity->addComponent<CAnimation>(name, animation);
            }
            else
            {
                std::cout << "Adding new animation of name: " << name << std::endl;
                entity->getComponent<CAnimation>().animations[name] = *animation;
                exit(1);
            }

            entity->addComponent<CBoundingBox>(animation->getFrameSize() * 4);
            entity->addComponent<CTransform>(gridToMidPixelBoundingBox(x, y, entity));
            entity->getComponent<CTransform>().scale = Vec2(4, 4);
        }
        else if (dataIn == "Dec")
        {
            std::string name; // needs to be valid animation asset
            float x, y;

            fin >> name >> x >> y;

            const std::shared_ptr<Entity>& entity = m_entityManager.addEntity("Tile");

            const std::shared_ptr<Animation>& animation = m_game->assets().getAnimation(name);

            if (!entity->hasComponent<CAnimation>())
            {
                entity->addComponent<CAnimation>(name, animation);
            }
            else
            {
                entity->getComponent<CAnimation>().animations[name] = *animation;
            }

            entity->addComponent<CTransform>(gridToMidPixelAnimation(x, y, entity));
            entity->getComponent<CTransform>().scale = Vec2(4, 4);
        }
        else if (dataIn == "Background")
        {
            std::string name;

            fin >> name;
            
            const std::shared_ptr<Entity>& entity = m_entityManager.addEntity("Background");

            const std::shared_ptr<Animation>& animation = m_game->assets().getAnimation(name);

            entity->addComponent<CAnimation>(name, animation);
            entity->addComponent<CTransform>(entity->getComponent<CAnimation>().animations.at(entity->getComponent<CAnimation>().currentAnimation).getFrameSize());
        }
        else
        {
            std::cout << "Unexpected level initialization!\n";
            exit(1);
        }
    }

    fin.close();

    // NOTE: All of the code below is a sample code which shows you how to
    //       set up and use entities with the new syntax,a it should be removed

    spawnPlayer();

    spawnEnemy();

    auto enemy = m_entityManager.addEntity("Enemy");

    enemy->addComponent<CBoundingBox>(Vec2(64, 64));
    enemy->addComponent<CTransform>(Vec2(64, 64));


    /*
    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT: always add the CAnimation component first so that gridToMidPixel can compute correctly
    brick->addComponent<CAnimation>(m_game->assets().getAnimation("Brick"), true);
    brick->addComponent<CTransform>(Vec2(96, 480));
    // NOTE: Your final code should position the entity with the grid x,y position read from the file:
    // brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick))

    if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
    {
        std::cout << "This could be a good way of identifying if a tile is a brick!\n";
    }

    auto block = m_entityManager.addEntity("tile");
    block->addComponent<CAnimation>(m_game->assets().getAnimation("Block"), true);
    block->addComponent<CTransform>(Vec2(224, 480));
    // Add a bounding box, this will now show up if we press the 'c' key
    block->addComponent<CBoundingBox>(m_game->assets().getAnimation("Block").getSize());

    auto question = m_entityManager.addEntity("tile");
    question->addComponent<CAnimation>(m_game->assets().getAnimation("Question"), true);
    question->addComponent<CTransform>(Vec2(352, 480));
    */
    
    // NOTE: Components now returned as references, not pointers. So to modify actual component or something, this is proper way:
    // auto& transform = entity->getComponent<CTransform>()
    // then any changes made to transform will reflect in the original entity
}

void ScenePlay::onEnd()
{
    m_game->window().clear(sf::Color(0, 0, 0, 255));
    m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}

void ScenePlay::update()
{
    m_entityManager.update();
    //std::cout << m_entityManager.totalEntities() << std::endl;

    if (m_paused)
    {
        sRender();
    }
    else
    {
        sMovement();
        sCollision();
        sLifespan();
        sAnimation();
        sRender();
        std::cout << "Is player in air?: " << m_player->getComponent<CState>().midAir << std::endl;
        std::cout << "Prevpos: " << m_player->getComponent<CTransform>().prevPos << std::endl;
        std::cout << "Pos: " << m_player->getComponent<CTransform>().pos << std::endl;
    }

    /*
    for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities())
    {
        if (entity->tag() == "Player")
        {
            CTransform& playerTransform = entity->getComponent<CTransform>();
            std::cout << playerTransform.pos << ", " << playerTransform.velocity << std::endl;
        }
    }
    */

    m_currentFrame++;
    std::cout << m_currentFrame << std::endl;
}



void ScenePlay::sMovement()
{
    // TODO: Implement player movement / jumping based on its CInput component
    // Implement gravity's effect on the player
    // Implement the maximum player speed in both X and Y directions
    // Setting an entity's scale.x to -1,1 will make it face to the left/right

    for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities())
    {
        if (entity->hasComponent<CGravity>() && entity->hasComponent<CTransform>())
        {
            CTransform& transform = entity->getComponent<CTransform>();

            transform.prevPos = transform.pos;

            transform.velocity.setY(transform.velocity.getY() + entity->getComponent<CGravity>().gravity);

            if (transform.velocity.getY() > m_playerConfig.MAXSPEED)
            {
                transform.velocity.setY(m_playerConfig.MAXSPEED);
            }

            if (transform.velocity.getX() > m_playerConfig.MAXSPEED)
            {
                transform.velocity.setX(m_playerConfig.MAXSPEED);
            }

            if (transform.velocity.getX() < - m_playerConfig.MAXSPEED)
            {
                transform.velocity.setX(- m_playerConfig.MAXSPEED);
            }

            if (transform.pos.getY() > m_game->window().getSize().y)
            {
                transform.pos = gridToMidPixelBoundingBox(3, 2, entity);
            }

            if (entity->hasComponent<CBoundingBox>())
            {
                CBoundingBox& boundingBox = entity->getComponent<CBoundingBox>();

                std::cout << boundingBox.size << std::endl;
                std::cout << transform.pos << std::endl;

                if (transform.pos.getX() < boundingBox.halfSize.getX())
                {
                    transform.pos.setX(boundingBox.halfSize.getX());
                }
            }

            if (entity->hasComponent<CInput>() && entity->hasComponent<CState>())
            {
                CInput& entityInput = entity->getComponent<CInput>();
                CState& entityState = entity->getComponent<CState>();

                int direction = 0;

                if (entityInput.left && entityInput.right) { 
                    direction = 0; 
                    
                    if (entityInput.down)
                    {
                        entityState.state = "Ducking";
                    }
                    else
                    {
                        entityState.state = "Idle";
                    }
                }
                else if (entityInput.right) 
                { 
                    direction = 1; 

                    entity->getComponent<CState>().flipped = false;

                    entity->getComponent<CTransform>().scale = Vec2(3, 3); 

                    if (entityInput.down && entityInput.canSlide)
                    {
                        entityState.state = "Sliding";
                        entityState.slideTimer = 15;
                        entityInput.canSlide = false;
                    }
                    else
                    {
                        entityState.state = "Running";
                    }
                }
                else if (entityInput.left) 
                { 
                    direction = -1; 

                    entity->getComponent<CTransform>().scale = Vec2(-3, 3); 

                    entity->getComponent<CState>().flipped = true;

                    if (entityInput.down && entityInput.canSlide)
                    {
                        entityState.state = "Sliding";
                        entityState.slideTimer = 15;
                        entityInput.canSlide = false;
                    }
                    else
                    {
                        entityState.state = "Running";
                    }
                }
                else { 
                    direction = 0; 

                    if (entityInput.down)
                    {
                        entityState.state = "Ducking";
                    }
                    else
                    {
                        entityState.state = "Idle";
                    }
                }

                if (entityInput.up && entityInput.canJump && !entityState.midAir)
                {
                    transform.velocity += Vec2(0, - m_playerConfig.JUMP);

                    entityState.state = "Jumping";
                    entityState.jumpTimer = 15;

                    entityInput.canJump = false;
                }
                else if (entityState.jumpTimer > 0)
                {
                    entityState.state = "Jumping";
                }

                if (entityInput.swingSword && entityInput.canSwingSword)
                {
                    if (entityState.midAir)
                    {
                        entityState.state = "SwingSword3";
                        entityState.swingSwordTimer = 25;
                    }
                    else
                    {
                        if (!entityState.flipped)
                        {
                            entityState.state = "SwingSword1";
                            entityState.swingSwordTimer = 25;
                        }
                        else
                        {
                            entityState.state = "SwingSword2";
                            entityState.swingSwordTimer = 25;
                        }
                    }

                    entityInput.canSwingSword = false;

                    spawnSword(entity);
                }
                else if (entityState.swingSwordTimer > 0)
                {
                    entityState.state = "SwingSword1";
                }

                if (entityState.midAir)
                {
                    transform.velocity += Vec2(direction * m_playerConfig.SPEED * m_playerConfig.DAMPING, 0);
                }
                else
                {
                    transform.velocity += Vec2(direction * m_playerConfig.SPEED, 0);
                }

                if (entityState.slideTimer > 0)
                {
                    transform.velocity += Vec2(direction * m_playerConfig.SPEED / 2, 0);
                    std::cout << "Slide timer: " << entityState.slideTimer << std::endl;
                }

                if (!entityState.midAir && ((!entityInput.left && !entityInput.right) || (entityInput.left && entityInput.right)))
                {
                    transform.velocity.setX(0);
                }
            }

            transform.pos += transform.velocity;

            std::cout << "Entity velocity: " << transform.velocity << std::endl;

            //std::cout << "Position: " << transform.pos << std::endl;
            //std::cout << "Previous Position: " << transform.prevPos << std::endl;
            
        }


        if (entity->tag() == "Sword")
        {
            int flipped = (m_player->getComponent<CState>().flipped) ? -1 : 1 ;

            CTransform& swordTransform = entity->getComponent<CTransform>();
            CBoundingBox& swordBoundingBox = entity->getComponent<CBoundingBox>();

            if (flipped == 1)
            {
                swordTransform.pos = m_player->getComponent<CTransform>().pos;
                swordTransform.pos.setX(swordTransform.pos.getX() + 0.5 * swordBoundingBox.size.getX());
            }
            else
            {
                swordTransform.pos = m_player->getComponent<CTransform>().pos;
                swordTransform.pos.setX(swordTransform.pos.getX() - 1.5 * swordBoundingBox.size.getX());
            }

            swordTransform.pos.setY(swordTransform.pos.getY() - 0.53 * swordBoundingBox.size.getY());
        }
    }
}

void ScenePlay::sLifespan()
{
    for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities())
    {
        if (entity->hasComponent<CLifespan>())
        {
            CLifespan& lifespan = entity->getComponent<CLifespan>();

            if (lifespan.frameCreated + lifespan.lifespan == static_cast<int>(m_currentFrame))
            {
                entity->destroy();
            }
        }

        if (entity->hasComponent<CHealth>())
        {
            if (entity->getComponent<CHealth>().health <= 0)
            {
                entity->destroy();
            }
        }
    }
}

void ScenePlay::sCollision()
{
    size_t totalChecks = 0;
    size_t totalCollisions = 0;
    size_t totalEntities = m_entityManager.getEntities().size();
    std::cout << "Total entities: " << totalEntities << std::endl;

    for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities())
    {
        if (entity->hasComponent<CGravity>() && entity->hasComponent<CBoundingBox>() && entity->hasComponent<CTransform>())
        {
            bool hasSupport = false;

            for (const std::shared_ptr<Entity>& otherEntity: m_entityManager.getEntities())
            {
                if (entity->id() != otherEntity->id() && otherEntity->hasComponent<CBoundingBox>() && otherEntity->hasComponent<CTransform>())
                {
                    CTransform& entityWithGravityTransform = entity->getComponent<CTransform>();

                    totalChecks++;

                    const Vec2& overlap = Physics::GetOverlap(entity, otherEntity);
                    const Vec2& previousOverlap = Physics::GetPreviousOverlap(entity, otherEntity);

                    if (overlap.getX() > 0 && overlap.getY() > 0)
                    {
                        totalCollisions++;

                        if ((entity->tag() == "Player" && otherEntity->tag() == "Enemy") || (entity->tag() == "Enemy" && entity->tag() == "Player"))
                        {
                            // TODO: Player takes damage and knockback
                        }

                        if ((entity->tag() == "Enemy" && otherEntity->tag() == "Sword") || (entity->tag() == "Sword" && otherEntity->tag() == "Enemy"))
                        {
                            // TODO: Enemy takes damage
                        }

                        //hasSupport = true;

                        int above = (entityWithGravityTransform.pos.getY() <= otherEntity->getComponent<CTransform>().pos.getY()) ? -1 : 1 ;

                        if (entity->hasComponent<CState>() && above == -1)
                        {
                            hasSupport = true;
                            entity->getComponent<CState>().midAir = false;
                        }

                        int right = (entityWithGravityTransform.pos.getX() > otherEntity->getComponent<CTransform>().pos.getX()) ? 1 : -1 ;

                        if (previousOverlap.getX() > 0 && previousOverlap.getY() <= 0)
                        {
                            // Resolve collision vertically
                            entityWithGravityTransform.pos.setY(entityWithGravityTransform.pos.getY() + above * overlap.getY());
                            entityWithGravityTransform.velocity.setY(0); // Stop vertical movement
                        }
                        else if (previousOverlap.getY() > 0 && previousOverlap.getX() <= 0)
                        {
                            // Resolve collision horizontally
                            entityWithGravityTransform.pos.setX(entityWithGravityTransform.pos.getX() + right * overlap.getX());
                            entityWithGravityTransform.velocity.setX(0); // Stop horizontal movement

                            //entity->getComponent<CInput>().right = false;
                            //entity->getComponent<CInput>().left = false;
                            entity->getComponent<CState>().midAir = true;
                        }
                        else if (previousOverlap.getX() > 0 && previousOverlap.getY() > 0)
                        {
                            // Minimum penetration axis
                            if (overlap.getX() < overlap.getY())
                            {
                                entityWithGravityTransform.pos.setX(entityWithGravityTransform.pos.getX() + right * overlap.getX());
                                entityWithGravityTransform.velocity.setX(0); // Stop horizontal movement
                            }
                            else
                            {
                                entityWithGravityTransform.pos.setY(entityWithGravityTransform.pos.getY() + above * overlap.getY());
                                entityWithGravityTransform.velocity.setY(0); // Stop vertical movement

                                // Additional logic to prevent climbing up vertical hitboxes
                                if (above < 0 && overlap.getX() < overlap.getY())
                                {
                                    entityWithGravityTransform.velocity.setX(0); // Prevent climbing up
                                }
                            }
                        }

                        entityWithGravityTransform.velocity.setX(0);
                        //entityWithGravityTransform.velocity = Vec2(0, 1);
                    }
                }
            }

            if (!hasSupport && entity->hasComponent<CState>())
            {
                entity->getComponent<CState>().midAir = true;
            }
        }
    }

    std::cout << "Total collision checks: " << totalChecks << std::endl;
    std::cout << "Total collisions: " << totalCollisions << std::endl;

    // NOTE: SFML's (0,0) position is on the top left corner
    // this means jumping will have a negative y-component
    // and gravity will have a positive y component
    // Also, something BELOW something else will have a y value GREATER than it
    // Something ABOVE something else will have a y value LESS than it
    //
    // TODO: IMPLEMENT Physics::GetOverlap() function, use it inside this function
    //
    // TODO: Implement bullet / tile collisions
    // Destroy the tile if it has a Brick animation
    // Implement player / tile collisions and resolutions
    // Update the CState component of the player to store whether it is currently
    // on the ground or in the air. This will be used by the Animation system
    // Check to see if the player has fallen down a hole (y > height())
    // Don't let the player walk off the left side of the map
}

void ScenePlay::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures ; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision ; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid ; }
        else if (action.name() == "PAUSE") { setPaused(!m_paused) ; }
        else if (action.name() == "QUIT") { onEnd() ; }
        else if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = true ; }
        else if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = true ; }
        else if (action.name() == "DUCK") 
        { 
            if (m_player->getComponent<CInput>().canSlide) { m_player->getComponent<CInput>().down = true; }
        }
        else if (action.name() == "JUMP") 
        { 
            if (m_player->getComponent<CInput>().canJump) { m_player->getComponent<CInput>().up = true ; }
        }
        else if (action.name() == "SWING_SWORD")
        {
            if (m_player->getComponent<CInput>().canSwingSword) { m_player->getComponent<CInput>().swingSword = true; }
        }
    }
    else if (action.type() == "END")
    {
        if (action.name() == "MOVE_LEFT") { m_player->getComponent<CInput>().left = false ; }
        else if (action.name() == "MOVE_RIGHT") { m_player->getComponent<CInput>().right = false ; }
        else if (action.name() == "DUCK") { m_player->getComponent<CInput>().down = false ; m_player->getComponent<CInput>().canSlide = true ; }
        else if (action.name() == "JUMP") { m_player->getComponent<CInput>().up = false ; m_player->getComponent<CInput>().canJump = true ; }
        else if (action.name() == "SWING_SWORD") { m_player->getComponent<CInput>().swingSword = false; m_player->getComponent<CInput>().canSwingSword = true;}
    }
}

void ScenePlay::sAnimation()
{
    // TODO: set the animation of the player based on its CState component
    // for each entity with an animation, call entity->getComponent<CAnimation>().animation.update()
    // if the animation is not repeated, and it has ended, destroy the entity
    // Iterate through all entities and update their animations if they have any
    for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities())
    {
        if (entity->hasComponent<CAnimation>())
        {
            CAnimation& entityAnimation = entity->getComponent<CAnimation>();
            auto it = entityAnimation.animations.find(entityAnimation.currentAnimation);

            if (it != entityAnimation.animations.end())
            {
                if (entity->hasComponent<CState>())
                {
                    CState& entityState = entity->getComponent<CState>();

                    if (entityState.swingSwordTimer > 0)
                    {
                        if (entityState.midAir)
                        {
                            entityAnimation.currentAnimation = "SwingSword3";
                        }
                        else
                        {
                            if (!entityState.flipped)
                            {
                                entityAnimation.currentAnimation = "SwingSword1";
                            }
                            else
                            {
                                entityAnimation.currentAnimation = "SwingSword2";
                            }
                        }

                        entityState.swingSwordTimer--;
                    }
                    else if (entityState.jumpTimer > 0)
                    {
                        entityAnimation.currentAnimation = "Jumping";
                        entityState.jumpTimer--;
                    }
                    else if (entityAnimation.currentAnimation == "Sliding" && entityState.slideTimer > 0)
                    {
                        entityAnimation.currentAnimation = "Sliding";
                        entityState.slideTimer--;
                    }
                    else
                    {
                        entityAnimation.currentAnimation = entityState.state;
                    }

                    if (entityAnimation.currentAnimation != "Sliding")
                    {
                        entityState.slideTimer = 0;

                        // TODO: MAKE THE PLAYER HITBOX SMALLER WHEN SLIDING
                    }
                    else
                    {
                    }

                    if (entityAnimation.currentAnimation != "SwingSword1" && entityAnimation.currentAnimation != "SwingSword2")
                    {
                        entityState.swingSwordTimer = 0;
                    }

                    if (!entityState.midAir)
                    {
                        entityState.jumpTimer = 0;
                    }

                std::cout << "Current animation: " << entityAnimation.currentAnimation << std::endl;
                }

                if (it->second.hasEnded())
                {
                    it->second.reset();
                }

                it->second.update();
            }
            else
            {
                std::cerr << "Error: Animation '" << entityAnimation.currentAnimation << "' not found in animations map." << std::endl;
            }

        }
    }
}

void ScenePlay::sRender()
{
    if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)) ; }
    else { m_game->window().clear(sf::Color(50, 50, 150)) ; }


    Vec2& playerPosition = m_player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, playerPosition.getX());
    sf::View view = m_game->window().getView();
    view.setCenter(windowCenterX, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);

    for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities("Background"))
    {
        if (entity->hasComponent<CAnimation>())
        {
            sf::Sprite& backgroundSprite = entity->getComponent<CAnimation>().animations.at(entity->getComponent<CAnimation>().currentAnimation).getSprite();

            float scaleX = static_cast<float>(m_game->window().getSize().x) / backgroundSprite.getTexture()->getSize().x;
            float scaleY = static_cast<float>(m_game->window().getSize().y) / backgroundSprite.getTexture()->getSize().y;

            backgroundSprite.setScale(scaleX, scaleY);
            backgroundSprite.setPosition(view.getCenter().x,
                                         view.getCenter().y);

            m_game->window().draw(backgroundSprite);
        }
    }

    if (m_drawTextures)
    {
        std::cout << "Drawing textures\n";
        for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities())
        {
            if (entity->tag() == "Background")
            {
                continue;
            }

            if (entity->hasComponent<CAnimation>())
            {
                CTransform& transform = entity->getComponent<CTransform>();
                std::string currentAnimation = entity->getComponent<CAnimation>().currentAnimation;

                auto animationIterator = entity->getComponent<CAnimation>().animations.find(currentAnimation);

                if (animationIterator != entity->getComponent<CAnimation>().animations.end())
                {
                    Animation& animation = animationIterator->second;
                    sf::Sprite& sprite = animation.getSprite();

                    sprite.setRotation(transform.angle);
                    sprite.setPosition(transform.pos.getX(), transform.pos.getY());
                    sprite.setScale(transform.scale.getX(), transform.scale.getY());

                    m_game->window().draw(sprite);
                }
                else
                {
                    std::cerr << "Warning: Animation '" << currentAnimation << "'not found for entity: " << entity->tag();
                }
            }
        }
    }

    // draw all entity collision bounding boxes with a rectangle shape
    if (m_drawCollision)
    {
        for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities())
        {
            if (entity->hasComponent<CBoundingBox>())
            {
                CBoundingBox& box = entity->getComponent<CBoundingBox>();
                CTransform& transform = entity->getComponent<CTransform>();

                sf::RectangleShape rectangle(sf::Vector2f(box.size.getX() - 1, box.size.getY()));
                rectangle.setOrigin(sf::Vector2f(box.halfSize.getX(), box.halfSize.getY()));
                rectangle.setPosition(transform.pos.getX(), transform.pos.getY());
                rectangle.setFillColor(sf::Color(0, 0, 0, 0));
                rectangle.setOutlineColor(sf::Color(255, 255, 255, 255));
                rectangle.setOutlineThickness(1);

                m_game->window().draw(rectangle);
            }
        }
    }

    // draw the grid to debug
    if (m_drawGrid)
    {
        float leftX = m_game->window().getView().getCenter().x - static_cast<float>(width()) / 2;
        float rightX = leftX + width() + m_gridSize.getX();
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.getX());

        for (float x = nextGridX; x < rightX; x += m_gridSize.getX())
        {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y = 0; y < height(); y += m_gridSize.getY())
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.getX())
            {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.getX());
                std::string yCell = std::to_string((int)y / (int)m_gridSize.getY());

                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.getY() + 2);

                m_game->window().draw(m_gridText);
            }
        }
    }

    m_game->window().display();
}

Vec2 ScenePlay::gridToMidPixelBoundingBox(float gridX, float gridY, const std::shared_ptr<Entity>& entity)
{
    float halfWidth = entity->getComponent<CBoundingBox>().halfSize.getX();
    float halfHeight = entity->getComponent<CBoundingBox>().halfSize.getY();

    return Vec2(gridX * m_gridSize.getX() + halfWidth, m_game->window().getSize().y - gridY * m_gridSize.getY() - halfHeight);
}

Vec2 ScenePlay::gridToMidPixelAnimation(float gridX, float gridY, const std::shared_ptr<Entity>& entity)
{
    std::string currentAnimation = entity->getComponent<CAnimation>().currentAnimation;

    Vec2 frameSize = entity->getComponent<CAnimation>().animations.at(currentAnimation).getFrameSize();

    float halfWidth = frameSize.getX() / 2;
    float halfHeight = frameSize.getY() / 2;

    return Vec2(gridX * m_gridSize.getX() + 4 * halfWidth, m_game->window().getSize().y - gridY * m_gridSize.getY() -  4 * halfHeight);

}

void ScenePlay::spawnPlayer()
{
    m_player = m_entityManager.addEntity("Player");

    const std::shared_ptr<Animation>& idleAnimation = m_game->assets().getAnimation("Idle");
    const std::shared_ptr<Animation>& runningAnimation = m_game->assets().getAnimation("Running");
    const std::shared_ptr<Animation>& duckingAnimation = m_game->assets().getAnimation("Ducking");
    const std::shared_ptr<Animation>& slidingAnimation = m_game->assets().getAnimation("Sliding");
    const std::shared_ptr<Animation>& jumpingAnimation = m_game->assets().getAnimation("Jumping");
    const std::shared_ptr<Animation>& swingSwordAnimation1 = m_game->assets().getAnimation("SwingSword1");
    const std::shared_ptr<Animation>& swingSwordAnimation2 = m_game->assets().getAnimation("SwingSword2");
    const std::shared_ptr<Animation>& swingSwordAnimation3 = m_game->assets().getAnimation("SwingSword3");
    std::cout << "FOUND PLAYER ANIMATION ASSET\n";

    std::unordered_map<std::string, Animation> animations;
    animations["Idle"] = *idleAnimation;
    animations["Running"] = *runningAnimation;
    animations["Ducking"] = *duckingAnimation;
    animations["Sliding"] = *slidingAnimation;
    animations["Jumping"] = *jumpingAnimation;
    animations["SwingSword1"] = *swingSwordAnimation1;
    animations["SwingSword2"] = *swingSwordAnimation2;
    animations["SwingSword3"] = *swingSwordAnimation3;

    m_player->addComponent<CAnimation>(animations, "Idle");

    std::cout << "SETTING PLAYER ANIMATION COMPONENT\n";

    Vec2 animationSize = m_player->getComponent<CAnimation>().animations.at(m_player->getComponent<CAnimation>().currentAnimation).getFrameSize();
    std::cout << "Just got animation size\n";
    Vec2 newAnimationSize = Vec2(animationSize.getX() * 0.3, animationSize.getY() * 0.9);

    std::cout << "Changed the size of animation\n";

    m_player->addComponent<CBoundingBox>(newAnimationSize * 3);

    Vec2 playerPos = gridToMidPixelBoundingBox(m_playerConfig.SPAWN_X, m_playerConfig.SPAWN_Y, m_player);

    m_player->addComponent<CTransform>(playerPos);
    m_player->getComponent<CTransform>().scale = Vec2(3, 3);

    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);

    m_player->addComponent<CInput>();
    m_player->addComponent<CState>();

    m_player->addComponent<CHealth>(10);


    // TODO: be sure to add the remaining components to the player
}

void ScenePlay::spawnSword(const std::shared_ptr<Entity>& entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is facing

    std::shared_ptr<Entity> sword = m_entityManager.addEntity("Sword");

    int flipped = (entity->getComponent<CState>().flipped) ? -1 : 1 ;

    CTransform& swordTransform = sword->addComponent<CTransform>();
    CBoundingBox& swordBoundingBox = sword->addComponent<CBoundingBox>();

    swordBoundingBox.size = Vec2(55, 100);

    if (flipped == 1)
    {
        swordTransform.pos = m_player->getComponent<CTransform>().pos;
        swordTransform.pos.setX(swordTransform.pos.getX() + 0.5 * swordBoundingBox.size.getX());
    }
    else
    {
        swordTransform.pos = m_player->getComponent<CTransform>().pos;
        swordTransform.pos.setX(swordTransform.pos.getX() - 1.5 * swordBoundingBox.size.getX());
    }

    swordTransform.pos.setY(swordTransform.pos.getY() - 0.5 * swordBoundingBox.size.getY());

    sword->addComponent<CLifespan>(20, m_currentFrame);

    sword->addComponent<CDamage>(2, m_currentFrame, m_playerConfig.FRAMES_TO_DAMAGE);

    entity->getComponent<CState>();
}

void ScenePlay::spawnEnemy()
{
    std::shared_ptr<Entity> enemy = m_entityManager.addEntity("Enemy");

    enemy->addComponent<CHealth>(6);
    enemy->addComponent<CDamage>(2, m_currentFrame, 10);


    enemy->addComponent<CBoundingBox>(Vec2(64, 64));

    CTransform& transform = enemy->addComponent<CTransform>();
    transform.pos = gridToMidPixelBoundingBox(5, 1, enemy);

}
