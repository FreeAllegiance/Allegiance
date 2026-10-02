# .github/workflows

[Back to the index](../LLAMA_SUMMARY.md) - 1 files, 220 lines. Notes by a local model, to orient by: not verified facts.

The `.github/workflows` folder contains configuration files for GitHub Actions workflows used in the Allegiance project. These workflows automate tasks such as building and deploying the game.

- build-and-deploy.yml: Defines a workflow to build the Allegiance game from source code and deploy it to Steam.

## `.github/workflows/build-and-deploy.yml`

PURPOSE: This GitHub Actions workflow is designed to build and deploy the Allegiance game from source code to Steam.

KEY PARTS:
- Sets up SteamCMD for interacting with Steam.
- Checks out the Allegiance and AllegianceSecurity repositories.
- Handles Steam Guard authentication by requesting and using a code.
- Restores NuGet packages and builds various components of the project.
- Copies security libraries to the Allegiance source directory.
- Builds the Allegiance game in FZRetail configuration.
- Stages build artifacts for deployment.
- Deploys the built game to Steam using SteamCMD.
- Updates a file checksum and uploads it for DRM purposes.

CONNECTS TO: 
- Allegiance and AllegianceSecurity repositories
- SteamCMD for Steam interactions
- GitHub secrets for authentication and other credentials

NOTES:
- The workflow is triggered on pushes to specific branches or manually via the workflow_dispatch action.
- Some steps are commented out, indicating they were part of a previous setup but are no longer used.
- Environment variables like STEAM_USERNAME and STEAM_PASSWORD are stored as secrets in GitHub.
