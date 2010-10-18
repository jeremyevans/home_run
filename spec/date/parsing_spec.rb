require File.expand_path('../../spec_helper', __FILE__)

ruby_version_is "1.9" do
  describe "Date parsing methods" do
    it "._httpdate should parse an HTTP format" do
      Date._httpdate("Fri, 02 Jan 2009 00:00:00 GMT").should ==
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>0, :min=>0, :sec=>0, :offset=>0, :zone=>'GMT'}
      Date._httpdate("Friday, 02-Jan-09 00:00:00 GMT").should ==
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>0, :min=>0, :sec=>0, :offset=>0, :zone=>'GMT'}
      Date._httpdate("Fri Jan 2 00:00:00 2009").should == 
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>0, :min=>0, :sec=>0}
    end

    it "._iso8601 should parse an ISO8601 format" do
      Date._iso8601("2009-01-02").should == {:year=>2009, :mon=>1, :mday=>2}
      Date._iso8601("--1002").should == {:mon=>10, :mday=>2}
    end

    it "._jisx03010 should parse an JIS X 0301 format" do
      Date._jisx0301("H21.01.02").should == {:year=>2009, :mon=>1, :mday=>2}
      Date._jisx0301("S63.01.02").should == {:year=>1988, :mon=>1, :mday=>2}
      Date._jisx0301("T14.01.02").should == {:year=>1925, :mon=>1, :mday=>2}
      Date._jisx0301("M44.01.02").should == {:year=>1911, :mon=>1, :mday=>2}
      Date._jisx0301("M06.01.02").should == {:year=>1873, :mon=>1, :mday=>2}
      Date._jisx0301("1872-01-02").should == {:year=>1872, :mon=>1, :mday=>2}
      Date._jisx0301("1867-01-02").should == {:year=>1867, :mon=>1, :mday=>2}

      Date._jisx0301("21.01.02").should == {:year=>2009, :mon=>1, :mday=>2}
    end

    it "._rfc2822 should parse an RFC2822 format" do
      Date._rfc2822("Fri, 2 Jan 2009 00:00:00 +0000").should == 
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>0, :min=>0, :sec=>0, :offset=>0, :zone=>'+0000'}
      Date._rfc2822("Fri, 2 Jan 09 00:00:00 +0000").should == 
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>0, :min=>0, :sec=>0, :offset=>0, :zone=>'+0000'}
      Date._rfc2822("Fri, 2 Jan 109 00:00:00 +0000").should == 
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>0, :min=>0, :sec=>0, :offset=>0, :zone=>'+0000'}
    end

    it "._rfc822 should parse an RFC822 format" do
      Date._rfc822("Fri, 2 Jan 2009 00:00:00 +0000").should == 
        {:year=>2009, :mon=>1, :mday=>2, :wday=>5, :hour=>0, :min=>0, :sec=>0, :offset=>0, :zone=>'+0000'}
    end

    it "._rfc3339 should parse an RFC3339 format" do
      Date._rfc3339("2009-01-02T00:00:00+00:00").should == 
        {:year=>2009, :mon=>1, :mday=>2, :hour=>0, :min=>0, :sec=>0, :offset=>0, :zone=>'+00:00'}
    end

    it "._xmlschema should parse an ISO8601 format" do
      Date._xmlschema("2009-01-02").should == {:year=>2009, :mon=>1, :mday=>2}
    end

    it ".httpdate should parse an HTTP format" do
      Date.httpdate("Fri, 02 Jan 2009 00:00:00 GMT").should == Date.new(2009, 1, 2)
    end

    it ".iso8601 should parse an ISO8601 format" do
      Date.iso8601("2009-01-02").should == Date.new(2009, 1, 2)
    end

    it ".jisx03010 should parse an JIS X 0301 format" do
      Date.jisx0301("H21.01.02").should == Date.new(2009, 1, 2)
      Date.jisx0301("S63.01.02").should == Date.new(1988, 1, 2)
      Date.jisx0301("T14.01.02").should == Date.new(1925, 1, 2)
      Date.jisx0301("M44.01.02").should == Date.new(1911, 1, 2)
      Date.jisx0301("M06.01.02").should == Date.new(1873, 1, 2)
      Date.jisx0301("1872-01-02").should == Date.new(1872, 1, 2)
      Date.jisx0301("1867-01-02").should == Date.new(1867, 1, 2)
    end

    it ".rfc2822 should parse an RFC2822 format" do
      Date.rfc2822("Fri, 2 Jan 2009 00:00:00 +0000").should == Date.new(2009, 1, 2)
    end

    it ".rfc822 should parse an RFC822 format" do
      Date.rfc822("Fri, 2 Jan 2009 00:00:00 +0000").should == Date.new(2009, 1, 2)
    end

    it ".rfc3339 should parse an RFC3339 format" do
      Date.rfc3339("2009-01-02T00:00:00+00:00").should == Date.new(2009, 1, 2)
    end

    it ".xmlschema should parse an ISO8601 format" do
      Date.xmlschema("2009-01-02").should == Date.new(2009, 1, 2)
    end

    it "should keep the same class as the receiver" do
      c = Class.new(Date)
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
