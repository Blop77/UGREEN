SKILL   = lecture-revision-kit
KIT     ?= $(HOME)/.claude/skills/uliya-skill-kit
DIST    = dist

.PHONY: check package clean

# Run the official UGOS static checker (needs uliya-skill-kit installed),
# then our own check that every {value} in SKILL.md is defined and used.
check:
	node $(KIT)/scripts/check-skill.mjs skill/$(SKILL)
	python3 tools/lint_skill.py skill/$(SKILL)/SKILL.md

# Build the Round 1 Pre-test Skill Package. The ZIP holds only the skill folder.
package: check
	mkdir -p $(DIST)
	rm -f $(DIST)/$(SKILL).zip
	cd skill && zip -rX ../$(DIST)/$(SKILL).zip $(SKILL)
	@unzip -l $(DIST)/$(SKILL).zip

clean:
	rm -rf $(DIST)
