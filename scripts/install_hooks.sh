project_root=$(git rev-parse --show-toplevel)

cp "$project_root/scripts/pre-commit" "$project_root/.git/hooks"
