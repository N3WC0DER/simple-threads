# Contribution Guidelines

**simple-threads** is an open source project, and contributions from the community are welcomed, as long as they comply with our quality standards and licensing.

Code contributions must be submitted using [GitHub Pull Requests](https://github.com/pmmp/PocketMine-MP/pulls), where they will be reviewed by maintainers.

## Useful documentation from github.com
- [About pull requests](https://docs.github.com/en/github/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests)
- [About forks](https://docs.github.com/en/github/collaborating-with-pull-requests/working-with-forks/about-forks)
- [Creating a pull request from a fork](https://docs.github.com/en/github/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request-from-a-fork)

## Other things you'll need
- [git](https://git-scm.com/)

## Choosing a target branch

**simple-threads** has four primary branches of development:
- master
- major
- minor
- patch

Increase the version numbers in any examples files and the `README.md` to the new version that this Pull Request would represent. The versioning scheme we use is [SemVer](http://semver.org/).

## Making a pull request
The basic procedure to create a pull request is:
1. [Fork the repository on GitHub](https://github.com/pmmp/PocketMine-MP/fork). This gives you your own copy of the repository to make changes to.
2. Create a branch on your fork for your changes.
3. Make the changes you want to make on this branch.
4. You can then make a [pull request](https://github.com/pmmp/PocketMine-MP/pull/new) to the project.

## Pull request reviews
Pull requests will be reviewed by maintainers when they are available.
Note that there might be a long wait time before a reviewer looks at your PR.

Depending on the changes, maintainers might ask you to make changes to the PR to fix problems or to improve the code.
**Do not delete your fork** while your pull request remains open, otherwise you won't be able to make any requested changes and the PR will end up being declined.

### Requirements
The following are required as a minimum for pull requests. PRs that don't meet these requirements will be declined unless updated to meet them.

#### Licensing
**simple-threads** is licensed under [GPLv3 license](LICENSE).
By proposing a pull request, you agree to your code being distributed within **simple-threads** under the same license.

#### PRs should be about exactly ONE thing
If you want to make multiple changes, those changes should each be contributed as separate pull requests. **DO NOT** mix unrelated changes.

#### PRs must not include unnecessary/unrelated changes
Do not include changes which aren't strictly necessary. This makes it harder to review a PR, because the code diff becomes larger and harder to review.
This means:
- don't reformat or rearrange existing code
- don't change things that aren't related to the PR's objective
- don't rewrite existing code just to make it "look nicer"

#### Tests must be provided
Where possible, Google tests should be written for new or changed code.
**Simply saying "Tested" is not acceptable** and will lead to your PR being declined.

#### Comments and documentation must be written in American English
English is the shared languages of all current maintainers.

#### Code must be in the simple-threads style
It's your responsibility to ensure your code matches the formatting and styling of the rest of the code.
Run clang-format to format the code.

### Recommendations

- **Do not edit code directly on github.com.** We recommend learning how to use [`git`](https://git-scm.com). `git` allows you to "clone" a repository onto your computer, so that you can make changes using an IDE.
- **Use an IDE, not a text editor.** We recommend CLion or VSCode.
- **Create a new branch on your fork for each pull request.** This allows you to use the same fork to make multiple pull requests at the same time.
- **Use descriptive commit titles.** You can see an example [here](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html).
- **Do not include multiple unrelated changes in one commit.** An atomic style for commits is preferred - this means that changes included in a commit should be part of a single distinct change set. See [this link](https://www.freshconsulting.com/atomic-commits/) for more information on atomic commits. See the [documentation on `git add`](https://git-scm.com/docs/git-add) for information on how to isolate local changes for committing.
- **Your pull request will be checked and discussed in due time.** Since the team is scattered all around the world, your PR may not receive any attention for some time.

**Thanks for contributing to simple-threads!**