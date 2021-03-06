#include "stdafx.h"
#include "Augments.h"
#include "Level.h"
#include "Angel.h"
#include "Monster.h"
#include "Hero.h"
#include "utility.h"
#include <vector>

void Level::Initialize() {
  //theWorld.SetSideBlockers(true, 0.0f);
  /*auto t = new Platform();
  t->SetPosition(Vector2(MathUtil::ScreenToWorld(0,
                                                 utility::Window_height)));
  t->SetSize(250,1);
  t->SetColor(0,0,0);
  theWorld.Add(t);
  t->InitPhysics();*/
}
  
void Level::Clear() {
  for(int i = 0; i < platforms.size(); i++) {
    platforms[i]->Destroy();
  }
    
  platforms.clear();
}
  
Level::Platform::Platform() {
  SetFriction(0.6f);
  SetDensity(0.0f);
  SetRestitution(0.0f);
}



extern Actor* Level::BG_Scroll::bg_scroll1 = nullptr,* Level::BG_Scroll::bg_scroll2 = nullptr;
extern float Level::BG_Scroll::clamp = 0;

void Level::BG_Scroll::Update(float dt) {
  
  if ( bg_scroll1 != nullptr && bg_scroll2 != nullptr ) {
    
    clamp += dt*0.3;
    bg_scroll1->SetPosition(clamp-10,0);
    bg_scroll2->SetPosition(clamp-60.f,0);
    if ( clamp > 50 ) clamp = 0;
  }
}

std::vector<Level::Platform*> Level::platforms;

namespace LLeaves = Level::Leaves;

float LLeaves::rand_until_next_leave = -1;
float LLeaves::wind_speed = 0;
float LLeaves::wind_count = 0;

void LLeaves::Update(float t) {
  if ( rand_until_next_leave == -1 ) {
    auto t = new LLeaves::Leaf();
    theWorld.Add(t);
    rand_until_next_leave = utility::R_Rand()/70+1;
  }
  rand_until_next_leave -= t;
  if ( rand_until_next_leave - t <= 0 ) {
    rand_until_next_leave = -1; // gen next one
    int amount_of_leaves = int(utility::R_Rand())%2;
    while ( --amount_of_leaves != -1 ) {
      auto t = new LLeaves::Leaf();
      
      theWorld.Add(t);
    }
  }
  wind_count -= t;
  if ( wind_count <= 0 ) {
    wind_count = utility::R_Rand()/70+0.2;
    wind_speed = utility::R_Rand()-50;
  }

}


LLeaves::Leaf::Leaf() {
  transparency = 0.0f;
  lifetime = 5.0f;
  SetAlpha(transparency);
  anim_speed = 0.6;
  float x_offset = rand() % 8;
  SetPosition(Vector2(-6 - x_offset, 9));
  SetSize(.5f, .5f);
  SetDrawShape(ADS_Square);
  dip_up = 0;
  curr_anim = 0;
  //return;
  switch( int(utility::R_Rand())%4 ) {
    case 0:
      LoadSpriteFrames("Images\\leaf0r_000.png");
      anims = 6;
    break;
    case 1:
      LoadSpriteFrames("Images\\leaf1r_000.png");
      anims = 6;
    break;
    case 2:
      LoadSpriteFrames("Images\\leaf2r_000.png");
      anims = 8;
    break;
    case 3:
      LoadSpriteFrames("Images\\leaf3r_000.png");
      anims = 12;
    break;
    case 4:
      LoadSpriteFrames("Images\\leaf0l_000.png");
      anims = 6;
    break;
    case 5:
      LoadSpriteFrames("Images\\leaf1l_001.png");
      anims = 6;
    break;
    case 6:
      LoadSpriteFrames("Images\\leaf2l_000.png");
      anims = 8;
    break;
    case 7:
      LoadSpriteFrames("Images\\leaf3l_000.png");
      anims = 12;
    break;
  }
  InitPhysics();
  GetBody()->SetGravityScale(.01f);

  auto fixture = GetBody()->GetFixtureList()->GetFilterData();
  fixture.groupIndex = -8;
  GetBody()->GetFixtureList()->SetFilterData(fixture);
};


void LLeaves::Leaf::Update(float dt) {
  lifetime -= dt;

  anim_speed -= dt;
  if ( anim_speed <= 0 ) {
    anim_speed = 0.4;
    curr_anim = (curr_anim+1)%anims;
    SetSpriteFrame(curr_anim);
  }
  ApplyForce(Vector2(wind_speed*dt,0),Vector2(0,0));
  ApplyTorque(wind_speed*dt*dt);
  if (transparency < 1.0f && lifetime > 0.0f) {
    transparency += .004f;
  } else if (lifetime <= 0.0f) {
    transparency -= .008f;
    if ( GetBody()->GetLinearVelocity().y > -0.4f &&
         GetBody()->GetLinearVelocity().y <  0.4f   )
      transparency -= .08f;
  }
  SetAlpha(transparency);

  if (transparency <= 0) {
    Destroy();
  }
  
  // dip
  if ( dip_down <= 0 ) {
    dip_up -= dt;
    if ( dip_up <= 0 ) {
      dip_down = 25;
    }
    ApplyForce(Vector2(0,-dt),Vector2(0,0));
  } else {
    dip_down -= dt;
    if ( dip_down <= 0 ) {
      dip_up = 25;
    }
  }
}


Actor* Level::_HUD::hud_bottom,
            * Level::_HUD::hud_bitch,
            * Level::_HUD::hud_hero,
            * Level::_HUD::hud_monster;
TextActor* Level::_HUD::hero_health,
         * Level::_HUD::monster_health;
void Level::_HUD::Init() {
  hud_bottom = new Actor();
  hud_bottom->SetSize(MathUtil::PixelsToWorldUnits(480),
                      MathUtil::PixelsToWorldUnits(48));
  hud_bottom->SetSprite("Images\\hud_bottom.png");
  hud_bottom->SetPosition(0,-8.5);
  hud_bitch = new Actor();
  hud_bitch->SetSize(MathUtil::PixelsToWorldUnits(58),
                      MathUtil::PixelsToWorldUnits(42));
  hud_bitch->SetSprite("Images\\hud_bitchtits.png");
  hud_bitch->SetPosition(-50,-50);
  hud_hero = new Actor();
  hud_hero->SetSize(MathUtil::PixelsToWorldUnits(58),
                      MathUtil::PixelsToWorldUnits(42));
  hud_hero->SetSprite("Images\\hud_hero.png");
  hud_monster = new Actor();
  hud_monster->SetSize(MathUtil::PixelsToWorldUnits(58),
                      MathUtil::PixelsToWorldUnits(42));
  hud_monster->SetSprite("Images\\hud_enemy.png");

  hero_health = new TextActor();
  hero_health->SetPosition(-13,-12);
  monster_health = new TextActor();
  monster_health->SetPosition(13,-12);
  theWorld.Add(hud_bottom);
  theWorld.Add(hud_bitch);
  theWorld.Add(hud_monster);
  theWorld.Add(hud_hero);
  theWorld.Add(hero_health);
  theWorld.Add(monster_health);

}
void Level::_HUD::Apply() {
  if ( Hero::theEnemy != nullptr && Game::thePlayer != nullptr ) {
    hud_hero->SetPosition(-13.0,-8.5);
    hero_health->SetDisplayString(std::to_string(Hero::theEnemy->R_Health()));
    hero_health->SetPosition(-9,-8.3);
  } else {
    hud_hero->SetPosition(-50,-50);
    hero_health->SetDisplayString( "" );
    hero_health->SetPosition(-560,-50);
  }
  hud_monster->SetPosition(13.0,-8.5);
  if ( Game::thePlayer != nullptr ) {
    monster_health->SetDisplayString(std::to_string(Game::thePlayer->R_Health()));
    monster_health->SetPosition(6,-8.3);
  } else {
    monster_health->SetDisplayString( "" );
  }

}


/*void Level::Inventory::Load_New_Inventory(int level) {
  int num_items = (utility::R_Rand()*level)/50 + 1;
  if ( num_items > 14 ) num_items = 14;

  /*for ( int i = 0; i != num_items; ++ i )
    if ( items[i] == nullptr )
      items[i] = new Actor();_Item(int(int(utility::R_Rand())%int(_Item::size)));*/
//}