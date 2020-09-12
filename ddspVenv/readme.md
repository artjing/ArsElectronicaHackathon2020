#Follow Those steps to get DDSP running

```sh
pip3 install virtualenv virtualenvwrapper
brew update && brew upgrade
source ~/.bash_profile
mkvirtualenv ddspVenv -p python3
workon ddspVEnv
# cd into the ddspVEnv from wherever you are in the architecture.
cd ddspVenv
pip install -r requirementsVirtualEnveloppe.txt
pip install --upgrade ddsp
```


Then you can try to run the jupyter notebook:
