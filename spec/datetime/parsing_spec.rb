require File.expand_path('../../spec_helper', __FILE__)

ruby_version_is "1.9" do
  describe "DateTime parsing methods" do
    it "._httpdate should parse an HTTP format" do
      DateTime._httpdate("Fri, 02 Jan 2009 03:04:05 GMT").should ==
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>3, :min=>4, :sec=>5, :offset=>0, :zone=>'GMT'}
    end

    it "._iso8601 should parse an ISO8601 format" do
      DateTime._iso8601("2009-01-02T03:04:05+12:00").should == {:year=>2009, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._iso8601("03:04:05+12:00").should == {:hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._iso8601(" 030405.1+1200").should == {:hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+1200', :sec_fraction=>0.1}
    end

    it "._jisx03010 should parse an JIS X 0301 format" do
      DateTime._jisx0301("H21.01.02T03:04:05+12:00").should == {:year=>2009, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._jisx0301("S63.01.02T03:04:05+12:00").should == {:year=>1988, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._jisx0301("T14.01.02T03:04:05+12:00").should == {:year=>1925, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._jisx0301("M44.01.02T03:04:05+12:00").should == {:year=>1911, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._jisx0301("M06.01.02T03:04:05+12:00").should == {:year=>1873, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._jisx0301("1872-01-02T03:04:05+12:00").should == {:year=>1872, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._jisx0301("1867-01-02T03:04:05+12:00").should == {:year=>1867, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
    end

    it "._rfc2822 should parse an RFC2822 format" do
      DateTime._rfc2822("Fri, 2 Jan 2009 03:04:05 +1200").should == 
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+1200'}
    end

    it "._rfc822 should parse an RFC822 format" do
      DateTime._rfc822("Fri, 2 Jan 2009 03:04:05 +1200").should == 
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+1200'}
    end

    it "._rfc3339 should parse an RFC3339 format" do
      DateTime._rfc3339("2009-01-02T03:04:05+12:00").should == 
        {:year=>2009, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
    end

    it "._xmlschema should parse an ISO8601 format" do
      DateTime._xmlschema("2009-01-02T03:04:05+12:00").should == {:year=>2009, :mon=>1, :mday=>2, :hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._xmlschema("03:04:05+12:00").should == {:hour=>3, :min=>4, :sec=>5, :offset=>43200, :zone=>'+12:00'}
      DateTime._xmlschema("---01+12:00").should == {:mday=>1, :offset=>43200, :zone=>'+12:00'}
    end

    it ".httpdate should parse an HTTP format" do
      DateTime.httpdate("Fri, 02 Jan 2009 03:04:05 GMT").should == DateTime.new(2009, 1, 2, 3, 4, 5)
    end

    it ".iso8601 should parse an ISO8601 format" do
      DateTime.iso8601("2009-01-02T03:04:05+12:00").should == DateTime.new(2009, 1, 2, 3, 4, 5, 0.5)
    end

    it ".jisx03010 should parse an JIS X 0301 format" do
      DateTime.jisx0301("H21.01.02T03:04:05+12:00").should == DateTime.new(2009, 1, 2, 3, 4, 5, 0.5)
      DateTime.jisx0301("S63.01.02T03:04:05+12:00").should == DateTime.new(1988, 1, 2, 3, 4, 5, 0.5)
      DateTime.jisx0301("T14.01.02T03:04:05+12:00").should == DateTime.new(1925, 1, 2, 3, 4, 5, 0.5)
      DateTime.jisx0301("M44.01.02T03:04:05+12:00").should == DateTime.new(1911, 1, 2, 3, 4, 5, 0.5)
      DateTime.jisx0301("M06.01.02T03:04:05+12:00").should == DateTime.new(1873, 1, 2, 3, 4, 5, 0.5)
      DateTime.jisx0301("1872-01-02T03:04:05+12:00").should == DateTime.new(1872, 1, 2, 3, 4, 5, 0.5)
      DateTime.jisx0301("1867-01-02T03:04:05+12:00").should == DateTime.new(1867, 1, 2, 3, 4, 5, 0.5)
    end

    it ".rfc2822 should parse an RFC2822 format" do
      DateTime.rfc2822("Fri, 2 Jan 2009 03:04:05 +1200").should == DateTime.new(2009, 1, 2, 3, 4, 5, 0.5)
    end

    it ".rfc822 should parse an RFC822 format" do
      DateTime.rfc822("Fri, 2 Jan 2009 03:04:05 +1200").should == DateTime.new(2009, 1, 2, 3, 4, 5, 0.5)
    end

    it ".rfc3339 should parse an RFC3339 format" do
      DateTime.rfc3339("2009-01-02T03:04:05+12:00").should == DateTime.new(2009, 1, 2, 3, 4, 5, 0.5)
    end

    it ".xmlschema should parse an ISO8601 format" do
      DateTime.xmlschema("2009-01-02T03:04:05+12:00").should == DateTime.new(2009, 1, 2, 3, 4, 5, 0.5)
    end

    it "should keep the same class as the receiver" do
      c = Class.new(DateTime)
      c.httpdate("Fri, 02 Jan 2009 00:00:00 GMT").should be_kind_of(c)
      c.iso8601("2009-01-02").should be_kind_of(c)
      c.jisx0301("H21.01.02").should be_kind_of(c)
      c.rfc2822("Fri, 2 Jan 2009 00:00:00 +0000").should be_kind_of(c)
      c.rfc822("Fri, 2 Jan 2009 00:00:00 +0000").should be_kind_of(c)
      c.rfc3339("2009-01-02T00:00:00+00:00").should be_kind_of(c)
      c.xmlschema("2009-01-02").should be_kind_of(c)
    end
  end
end
