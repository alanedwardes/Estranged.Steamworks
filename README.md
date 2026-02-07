# EstSteam

Drop-in Steam integration for Unreal Engine. Requires no Blueprint or C++ changes to existing game logic.

## Functionality

*   **License Check**: Verifies ownership on startup. Exits if Steam is not running or the user does not own the application (Shipping/Standalone only).
*   **Web Overlay**: Redirects `FPlatformProcess::LaunchURL` calls to the Steam Overlay web browser.
*   **Player Name**: Overrides `APlayerState::GetPlayerName` to return the Steam persona name.

## Configuration

The module must load in `PostEngineInit` to avoid a race condition with `OnlineSubsystemSteam`, which initializes the Steam API during the `Default` phase.

**EstSteam.uplugin:**
```json
"LoadingPhase": "PostEngineInit"
```
