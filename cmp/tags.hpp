#pragma once
////////////////// TAG LIST
//
//


// PLAYER TAG --> PLAYER ACTIONS, PLAYER DIFFERENTIATION
struct TPlayer{};

// ENEMY TAG  --> ENEMY ACTIONS, ENEMY DIFFERENTIATION
struct TEnemy{};

// HEALTH TAG --> NOT MUCH USE, ENTITY HEALTH YES OR NOT
struct THealth{};

// INSTANCE TAG-> DIFFERENTIATION TAG FOR INSTANCE OBJECTS
struct TInstance{};

// COLLIDER TAG-> DIFFERENTIATION FOR A COLLIDER OBJECT
struct TCollider{};

// COLLISIONABLE TAG-> DIFFERENTIATION FOR A COLLISIONABLE OBJECT
struct TCollisionable{};

// NOT RENDER TAG -> IF THIS TAG EXIST , THE ENTITY IS NOT RENDERIZABLE
struct TNotRenderizable{};

// WALL TAG --> A TYPE OF COLLISION : WITH WALLS 
struct TWall {};

// PICKABLE TAG --> A TYPE OF COLLISION : OBJECT PICKABLE
struct Tpickable{};

// SOUND EVENT TAG --> A TYPE OF COLLISION : WHEN COLLISION A SOUND START 
struct TSEvent{};

//TAG FOR DOORS ---> TYPE OF COLLISION, CAN BE OPENED WITH A KEY
struct TDoor{};

//TAG FOR CHURCH KEY --- LIKE AN ID FOR THIS KEY
struct TDoorKeyChurch{};

//TAG FOR BURNABLE ITEMS
struct TBurnable{};

//TAG FOR ENTITIES WITH DESCRIPTION 
struct TDescription{};

//TAG TRANSICION DE NIVEL
struct TTransition{};

//TAG FOR SCALE BOUNDING BOX IN OBJECTS LIKE KEYS 
struct TScaleBoundingBox{};

//bounding box helper for other entities 
struct TBoundingBoxHelp{};

//TO IDENTIFY CONSUMABLE "YESQUERO" 
struct TYesquero{};

// Tag for lantern pick
struct TLantern{};

//Tag for reposition
struct TReposition{};

//Tag to save level status when pass the level
struct TSaveStatus{};

//Tag to do collision with window, and activate mailbox
struct TWindowMailBox{};

//Tag to do collision with mailbox and get a key
struct TMailBox{};

//Tag for print text in back door and make it interactuable
struct TBackDoor{};

//Tag to do bedroom gameplay 
struct TBedRoom{};

//Tag to fusible object
struct TFuse{};

//Tag to fusible box collision
struct TFuseBox{};

//Tag to reference phone in collision syst
struct TPhone{};

//Tag for house objects 
struct TObjectInteractuable{};

//OBJECTS TAGS
//Tag for object painting
struct TPainting{};
//COLLISION with farmer
struct TFarmer{};
//Tag to check if it needs animations
struct TAnimated{};

///Static enemy
struct TDummy{};
//TOMB EVENT 
struct TTombEvent{};
//enemy event in tombs
struct TEnemyEvent{};

//transition to lvl Y
struct TTransitionY{};

//caja fuerte
struct TSaveBox{};
//codelook
struct TCodeLock{};

//anillo 
struct TRing{};
//cizallas
struct TShears{};
//trampilla
struct THatch{};

struct TMarco{};

struct TJumpscare{};
struct TRueda{};

struct TEventDoorGranero{};

//El enemigo no patrullará
struct TNotPatrol{};

//evento final del coche, ganas o pierdes
struct TCarEventFinal{};

//No renderizar animation
struct TNotRenderAnim{};

//Yesqueros permanentes tanto en ida como en vuelta
struct TYesqueroPermanente{};