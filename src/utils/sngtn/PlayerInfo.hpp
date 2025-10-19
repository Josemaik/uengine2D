// Singleton PlayerInfo
enum class PlayerState {
  IDLE,
  RUNNING,
  JUMPING
};
struct PlayerInfo {

  PlayerState currentState = PlayerState::IDLE;
  vec2f position{ 0.f,0.f };
};