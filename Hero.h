#ifndef HERO_H_
#define HERO_H_

#include "Angel.h"
#include "LD33.h"
#include "Level.h"
#include <string>

namespace Hero {

  // abilities used by enemy heroes
  enum class Weapon { // 
    dagger, sword, broadsword, miniaxe, axe, bigaxe, spear, size
  };

  enum class Throwable { // 
    knife, cross, axe, ball, size
  };

  class E_Weapon : public PhysicsActor {
    Weapon weapon;
    float cooldown;
    bool hit;
  public:
    E_Weapon(Weapon);
    void Update(float);
  };

  class E_Throw : public PhysicsActor {
    Throwable type;
    float  cooldown;
    bool hit;
    Vector2 origin;
    bool recoiled;
  public:
    E_Throw(Throwable, float ang = 0);
    void Update(float);
  };

  class Enemy_Death : public PhysicsActor {
    float time;
  public:
    Enemy_Death();
    void Update(float);
  };

  class Enemy : public PhysicsActor {
    bool intro;
    float intro_time;
    float speed;
    int health;
    float slide_cooldown;
    std::string name;
    float movement_cooldown, melee_cooldown, range_cooldown;
    float movement_attack_flinch; // stops movement altogether after an attack

    float slide_timer;
    bool slide_direction;
    float ghost_timer;
    float ghost_cooldown;
    float platform_cooldown;
    float on_platform_timer;
    bool jumping_to_platform;    // hero is jumping up on top of the platform

    Level::Platform* platform;

    enum class Mood { // mood for how far they want to be
      Close, // only melee
      Fair, // could probably do a mix of ranged and melee
      Distant, // only ranged
      Size
    };

    Mood mood;
    TextActor* mood_tester;

    float mood_switch_timer;
    float jump_timer; // every 5 seconds we get a chance to jump
    float in_air_start, in_air_end; // used by attack. attack after start before end

    // takes care of cooldowns
    void Attack_Melee();
    // takes care of cooldowns
    void Attack_Range();

    void Apply_Vel_X(float x, float d);

    float gibber_timer;

    float walk_animation;

    enum class Anim_State {
      idle, jump, walk, slide, attack
    };

    Anim_State state;

    Actor* draw_me;
  public:

    E_Weapon* weapon;
    Weapon weapon_type;

    E_Throw* throwable;
    Throwable throw_type;

    bool wep_hit;
    
    float attack_animation;
    Enemy();
    ~Enemy();
    void Update(float dt);
    void Jump();
    void Add_Health(int x);
	  void Killed();
    inline int R_Health() { return health; }
  };

  /*// just moves into the screen all cool and then spawns Enemy
  class Enemy_Intro : public Actor {
  public:
    Enemy_Intro();
  };*/

  // used for enemy AI
  class Enemy_Listener : public MessageListener {
  public:
    void ReceiveMessage(Message* );
  };

  extern Enemy* theEnemy;
  //extern Enemy_Intro* theEnemyIntro;
  extern Enemy_Listener* e_listener;


  // enemy attacks
  class Dagger : public PhysicsActor {
    float lifetime;
    bool hit;
  public:
    Dagger(float angle, Vec2i position);
    void Update(float dt);
  };

  class Gold : public PhysicsActor {
    float amount;
  public:
	  Gold(Vector2 position);
    void Update(float dt);
  };
};

#endif